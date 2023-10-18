#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include <ArduinoJson.h>
#include <LittleFS.h>

#include "Data.hpp"

class ConfigManager
{
public:
    static void Init()
    {
        if (!LittleFS.begin())
        {
            Serial.println("An Error has occurred while mounting LittleFS");
            return;
        }

        LoadFromJson();
    }

    static void SaveToJson()
    {
        // Create a JSON document
        StaticJsonDocument<512> doc;

        JsonObject interface_data = doc.createNestedObject("interface_data");
        interface_data["gravity"] = Data::interface_data.gravity;
        interface_data["gravity_ratio"] = Data::interface_data.gravity_ratio;
        interface_data["orbit_rate"] = Data::interface_data.orbit_rate;
        interface_data["orbit_ratio"] = Data::interface_data.orbit_ratio;
        interface_data["scope_width"] = Data::interface_data.scope_width;
        interface_data["scope_phase"] = Data::interface_data.scope_phase;
        interface_data["ratio"] = Data::interface_data.ratio;

        JsonObject cv_data = doc.createNestedObject("cv_data");
        cv_data["input_a"] = Data::cv_data.input_a;
        cv_data["input_b"] = Data::cv_data.input_b;
        cv_data["input_c"] = Data::cv_data.input_c;
        cv_data["input_d"] = Data::cv_data.input_d;
        cv_data["cv_gravity"] = Data::cv_data.cv_gravity;
        cv_data["cv_orbit"] = Data::cv_data.cv_orbit;
        cv_data["cv_scope"] = Data::cv_data.cv_scope;
        cv_data["cv_ratio"] = Data::cv_data.cv_ratio;

        // Open the file for writing
        File file = LittleFS.open("/config_param.json", "w");
        if (!file)
        {
            Serial.println("Failed to open file for writing");
            return;
        }

        serializeJson(doc, file);

        // Close the file
        file.close();
    }

    static void LoadFromJson()
    {
        // Open the file for reading
        File file = LittleFS.open("/config_param.json", "r");
        if (!file)
        {
            Serial.println("Failed to open file for reading");
            return;
        }

        // Parse the JSON file and deserialize it into a JSON document
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, file);

        if (error)
        {
            Serial.println("Failed to deserialize JSON");
            return;
        }

        // Retrieve the data from the JSON document and update your data structure
        JsonObject interface_data = doc["interface_data"];
        Data::interface_data.gravity = interface_data["gravity"];
        Data::interface_data.gravity_ratio = interface_data["gravity_ratio"];
        Data::interface_data.orbit_rate = interface_data["orbit_rate"];
        Data::interface_data.orbit_ratio = interface_data["orbit_ratio"];
        Data::interface_data.scope_width = interface_data["scope_width"];
        Data::interface_data.scope_phase = interface_data["scope_phase"];
        Data::interface_data.ratio = interface_data["ratio"];

        JsonObject cv_data = doc["cv_data"];
        Data::cv_data.input_a = cv_data["input_a"];
        Data::cv_data.input_b = cv_data["input_b"];
        Data::cv_data.input_c = cv_data["input_c"];
        Data::cv_data.input_d = cv_data["input_d"];
        Data::cv_data.cv_gravity = cv_data["cv_gravity"];
        Data::cv_data.cv_orbit = cv_data["cv_orbit"];
        Data::cv_data.cv_scope = cv_data["cv_scope"];
        Data::cv_data.cv_ratio = cv_data["cv_ratio"];

        // Close the file
        file.close();
    }

private:
    // references to Data::interface_data, Data::cv_data:
};

#endif // CONFIGMANAGER_HPP
