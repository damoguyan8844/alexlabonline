#ifndef CBase64_H
#define CBase64_H

//class to handle all base64 stuff...
class CBase64
{
private:
  int ErrorCode;
public:
  int GetLastError() {return ErrorCode;};
	CBase64();
	~CBase64();
  char* Decode(char *input, int *bufsize); //caller must free the result, bufsize holds the decoded length
  char* Encode(char *input, int bufsize); //caller must free the result, bufsize is the length of the input buffer
};

#endif