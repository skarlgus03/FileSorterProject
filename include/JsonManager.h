#include <string>
#include <memory>
#include <nlohmann/json.hpp>


using json = nlohmann::json;



class JsonManager {
public:
	static bool saveToJson();
};