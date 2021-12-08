#include <iostream>
#include <map>
#include <vector>
#include <iterator>
#include <string>
#include <experimental/filesystem>
#include <filesystem> // no me funciona, tengo que usar el experimental

namespace fs = std::experimental::filesystem; // en caso de no necesitar el experimental/filesystem, cambiar esto
// namespace fs = std::filesystem;
using std::string;
// using namespace std; // ¡Peligro!

// lista de funciones
int main();
string pickDirectory();
void organizeDirectory();
void moveFile();

// Mover archivos a las carpetas correspondientes
void moveFile(string filePath, string directoryPath, string currentFile)
{
  // el nuevo Path del archivo
  string newFile = directoryPath + "\\" + currentFile;
  // accion de mover
  fs::rename(filePath, newFile);
};

// Selecciona el directorio en el que se colocaran los archivos
string pickDirectory(std::map<string, std::vector<string>> map, string str)
{
  // recorro el map
  for (std::map<string, std::vector<string>>::iterator iter = map.begin(); iter != map.end(); iter++)
  {
    // key
    string k = iter->first;
    // busco la posicion de la llave
    std::map<string, std::vector<string>>::const_iterator search = map.find(k);
    if (search != map.end())
    {
      // selecciono el vector que contiene las extensiones
      std::vector<string> v = search->second;
      // recorro el vector en busca de la extension str
      // y si no llega al final (lo encontro), devuelve "/key"
      if (std::find(v.begin(), v.end(), str) != v.end())
      {
        return "\\" + k;
      };
    };
  };
  // en caso de no encontrar la extension en el map, se utiliza la carpeta MISC
  return "\\MISC";
};

// Funcion principal
void organizeDirectory()
{
  // key = Tipo de Archivo | value = Extension Correspondiente
  std::map<string, std::vector<string>> subdirectories = {
      {"Documents", {".pdf", ".rtf", ".txt", ".md"}},
      {"Audio", {".m4a", ".m4b", ".mp3"}},
      {"Videos", {".mov", ".avi", ".mp4", ".flv", ".mkv"}},
      {"Images", {".jpg", ".jpeg", ".png", ".svg"}},
      {"Executables", {".pyc", ".exe", ".bat"}}};

  // carpeta actual
  string path = fs::current_path().string();
  // recorro cada archivo en el carpeta
  for (const auto &file : fs::directory_iterator(path))
  {
    // ignoro carpetas
    if (fs::is_directory(file))
    {
      continue;
    }
    // ignoro el executable
    string currentFile = file.path().filename().string();
    if (currentFile == "organizeDirectory.exe")
    {
      continue;
    }
    // el Path del archivo
    string filePath = file.path().string();
    // extension del archivo
    string fileType = filePath.substr(filePath.find_last_of("."));
    // to lower case
    transform(fileType.begin(), fileType.end(), fileType.begin(), ::tolower);
    // selecciono el directorio correspondiente y lo paso como string
    string directory = pickDirectory(subdirectories, fileType);
    // path de la carpeta actual + la carpeta seleccionada
    string directoryPath = fs::current_path().string() + directory;
    // si no existe la carpeta, creo una
    if (!fs::is_directory(directoryPath))
    {
      fs::create_directory(directoryPath);
    }
    moveFile(filePath, directoryPath, currentFile);
  }
};

int main()
{
  organizeDirectory();
  return 0;
}
