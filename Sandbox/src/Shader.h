#include <string>

class Shader {
  public: 
    /**
     * Shader Program ID
    */
    unsigned int ID;
    std::string vertexString;
    std::string fragmentString;
    const char *vertexSource;
    const char *fragmentSource;

    Shader(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;

  private:
    /**
     * 检测编译结果
     * */
    void checkCompileErrors(unsigned int shader, std::string type);
};