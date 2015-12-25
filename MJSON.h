#include "application.h"

class MJSON {
public:
    MJSON(void);
    void setJSON(String json);
    char* readValue(String key);
private:
	String jsonData;
};