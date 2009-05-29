/**
*  
*  @file vxFileGzipIo.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxFileGzipIo.h"


#include <zlib.h>

using namespace std;

Io::Io(string content): position_(0) {
  content_ = content;
  rewind();
};

Io::~Io(){};

  //technical
unsigned int Io::size(){return content_.size();};
Io & Io::rewind(){position_=0; return *this;};
unsigned int Io::get_position(){return position_;};

Io & Io::set_position( unsigned int position){
  if( position < 0 || position > content().size()){
    valid(false);
  }else{
    position_ = position;
  };
  return *this;
};

std::string Io::content(){return content_;};

  //Parametrized function to read different types using IO.
template<class T> 
Io & Read(Io & in, T * result){
  if ((in.content_.size() - in.position_)>=sizeof(T)) {
    for (unsigned int i = 0; i < sizeof(T); i++) {
      ((char *)result)[i] = in.content_[in.position_ + sizeof(T) - i - 1];
    };

    in.position_ += sizeof(T); // Updating position.
      } else {
    in.valid(false);
  };
  return in;
};

//If we do not want to use out-parameter.
template<class T>
T ReadDirectly(Io & in){
  T result = 0; //Make sure it is not garbage.
  Read<T>(in, &result);
  return result;
}; 

//Parametrized function to write different types using IO.
template<class T> 
Io & Write(Io & in, T * result){
  //Extend the string if current position is exactly at the end.
  if (in.get_position() == in.size()) //Append string of spaces.
    in.content_ += string (sizeof(T), ' '); 
  
  if ((in.size() - in.get_position()) >= sizeof(T)) { //Replace.
    for (unsigned int i = 0; i < sizeof(T); i++){ //Copy bytes.
      in.content_[in.get_position() + sizeof(T) - i - 1] = ((char *)result)[i];
    };
    in.position_ += sizeof(T); // Updating position.
  } else { //Too little space to replace and not appending; Error.
    in.valid(false);
  };

  return  in;   
}; 

  //reading
Io & Io::GetInt(int * result){ return Read<int>(*this, result); };
Io & Io::GetFloat(float * result){return Read<float>(*this, result); };
Io & Io::GetShort(short * result){return Read<short>(*this, result); };
Io & Io::GetChar(char * result){return Read<char>(*this, result); };

  //reading
int Io::GetInt(){ return ReadDirectly<int>(*this); };
float Io::GetFloat(){return ReadDirectly<float>(*this); };
short Io::GetShort(){return ReadDirectly<short>(*this); };
char Io::GetChar(){return ReadDirectly<char>(*this); };

  
  //writing
Io & Io::PutInt(int result){return Write<int>(*this, &result); };
Io & Io::PutFloat(float result){return Write<float>(*this, &result); };
Io & Io::PutShort(short result){return Write<short>(*this, &result); };
Io & Io::PutChar(char result){return Write<char>(*this, &result); };

//Auxiliary methods.

Io & Io::ReplaceContent(string new_content){
  content_ = new_content;
  rewind();
  return * this;
};

//File io.

string ReadFile(string name){
  FILE* file = fopen(name.c_str(), "rb");
  if (file == NULL) return "";

  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  rewind(file);

  char* chars = new char[size];

  for (int i = 0; i < size;) {
    int read = fread(&chars[i], 1, size - i, file);
    i += read;
  }
  fclose(file);
  string result(chars, size);
  delete[] chars;
  
  return result;
};


string ReadGzipFile(string name){ 

  gzFile fd; //file descriptor

  const int BUF_SIZE=16384;
  char buf [BUF_SIZE]; //for reading from the file
  
  std::string contents; //vector of strings, to be composed later into one vec.

  fd = gzopen(name.c_str(),"rb");
  if(fd == NULL) return ""; //cannot read file.
  
  
  int cnt = 0;

  while((cnt = gzread(fd, (void *) buf, BUF_SIZE))){
	contents+=std::string((char *)buf, cnt);
  };
  ::gzclose(fd);  

  printf("Acqured %d bytes.\n", (int)contents.size());

  //getting additional information
  //  get_mgz_info(name);

  return contents;  
};


//Write file
bool WriteFile( std::string name, std::string contents){
  FILE* file = fopen(name.c_str(), "wb");
  if (file == NULL) return false;
  unsigned int bytes_written = fwrite(contents.c_str(), 
			     1, 
			     contents.size(), 
			     file);
  fclose(file);
  return bytes_written == contents.size();
};

bool WriteGzipFile( std::string name, std::string contents){
  printf("file %s\n",  name.c_str());
  gzFile fd = gzopen(name.c_str(), "wb");
  //still cannot - fail:
  if(fd == NULL) return false; // Opening file failed.
  int cnt = gzwrite(fd, contents.c_str(), contents.size());
  gzflush(fd, Z_FINISH);
  gzclose(fd);
  return ((unsigned int)cnt == contents.size());
};


// End of vxFileGzipIo.cpp
