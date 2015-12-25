
#include "MJSON.h"

/**
* Constructor.
*/
MJSON::MJSON()
{

}


void MJSON::setJSON(String json) {
	this->jsonData = json;
}

char * MJSON::readValue(String key){
	char *source = (char *) jsonData.c_str();  
	char *dest = (char *) key.c_str();
	char *found = strstr(source, dest);

	char *midToken = strstr(found,":");



	char obj[200];
	// int index = 0;
	int indexEnd = 0;
	int indexMid = 0;
	char *endToken;
	if (found != NULL && midToken != NULL)
	{

		indexMid = midToken - source + 1;

		// Serial.print("midToken: ");
		// Serial.println(midToken);

		if (source[indexMid] == '"')
		{
			endToken = strstr(midToken + 2,"\"");

		}
		else{
			if (strstr(found,"}") < strstr(found,","))
			{
				endToken = strstr(found,"}");
			}
			else{

				endToken = strstr(found,",");
			}
		}

		if (endToken != NULL){

			indexEnd = endToken - source + 1;

			if (source[indexMid] == '"')
			{
				indexMid++;
			}

			int length = (indexEnd - indexMid);

			strlcpy(obj, &source[indexMid], length);
			Serial.print(key + ": ");
			Serial.println(obj);
		}



	}
	return obj;

}
