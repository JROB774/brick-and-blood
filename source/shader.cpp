INTERNAL GLuint CompileShaderStage (const std::string& source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    const char* c_source = source.c_str();
    glShaderSource(shader, 1, &c_source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint info_log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        std::string info_log;
        info_log.resize(info_log_length);
        glGetShaderInfoLog(shader, info_log_length, NULL, &info_log[0]);
        LOG_ERROR(ERR_MAX, "Failed to compile shader:\n%s", info_log.c_str());
    }

    return shader;
}

INTERNAL void LoadShader (Shader& shader, std::string file_name)
{
    // Load the shader file source code into a stream.
    std::ifstream file(file_name);
    if (!file.is_open())
    {
        LOG_ERROR(ERR_MAX, "Failed to load shader! (%s)", file_name.c_str());
        return;
    }
    std::stringstream stream;
    stream << file.rdbuf();

    std::string line;

    std::string vert_source;
    std::string geom_source;
    std::string frag_source;

    // By default we put the contents into both shaders. It is only once we reach one of the
    // attribute lines [VertProgram], [FragProgram], etc. that we place lines in a specific shader.
    bool in_vert_program = true;
    bool in_geom_program = true;
    bool in_frag_program = true;

    // Geometry shaders are optional and only if a [GeomProgram] attribute is found will we actually compile and link one.
    bool has_geom_program = false;

    while (std::getline(stream, line))
    {
        line.erase(0,line.find_first_not_of(" \t\n\r\f\v")); // Trim leading whitespace.

        // Handle our attributes.
        if      (line == "[VertProgram]") in_vert_program = true,  in_geom_program = false, in_frag_program = false;
        else if (line == "[GeomProgram]") in_vert_program = false, in_geom_program = true,  in_frag_program = false, has_geom_program = true;
        else if (line == "[FragProgram]") in_vert_program = false, in_geom_program = false, in_frag_program = true;
        else {
            // Add lines to the appropriate shader sources.
            if (in_vert_program) vert_source += line + "\n";
            if (in_geom_program) geom_source += line + "\n";
            if (in_frag_program) frag_source += line + "\n";
        }
    }

    GLuint vert = CompileShaderStage(vert_source, GL_VERTEX_SHADER);
    GLuint frag = CompileShaderStage(frag_source, GL_FRAGMENT_SHADER);
    GLuint geom = GL_NONE;
    if (has_geom_program)
    {
        geom = CompileShaderStage(geom_source, GL_GEOMETRY_SHADER);
    }

    shader.program = glCreateProgram();
    glAttachShader(shader.program, vert);
    glAttachShader(shader.program, frag);
    if (has_geom_program)
    {
        glAttachShader(shader.program, geom);
    }
    glLinkProgram(shader.program);

    GLint success;
    glGetProgramiv(shader.program, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint info_log_length;
        glGetProgramiv(shader.program, GL_INFO_LOG_LENGTH, &info_log_length);
        std::string info_log;
        info_log.resize(info_log_length);
        glGetProgramInfoLog(shader.program, info_log_length, NULL, &info_log[0]);
        LOG_ERROR(ERR_MAX, "Failed to link shader:\n%s", info_log.c_str());
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    if (has_geom_program)
    {
        glDeleteShader(geom);
    }
}

INTERNAL void FreeShader (Shader& shader)
{
    glDeleteProgram(shader.program);
}
