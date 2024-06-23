#include <inja/inja.hpp>
#include <iostream>
#include <string>

using namespace inja;

int main()
{
    std::cout << "Test External" << std::endl;

    json data;
    data["name"] = "world";

    render("Hello {{ name }}!", data);                 // Returns std::string "Hello world!"
    render_to(std::cout, "Hello {{ name }}!", data);   // Writes "Hello world!" to stream


    {
        // For more advanced usage, an environment is recommended.
        Environment env;
        {
            // Render a string with json data
            std::string result = env.render("Hello {{ name }}!", data);   // "Hello world!"
        }
        Template temp = env.parse_template("./templates/greeting.txt");
        {
            // Or directly read a template file
            std::string result = env.render(temp, data);   // "Hello world!"
        }
        data["name"]       = "Inja";
        std::string result = env.render(temp, data);   // "Hello Inja!"


        // Or read the template file (and/or the json file) directly from the environment
        result = env.render_file("./templates/greeting.txt", data);
        result = env.render_file_with_json_file("./templates/greeting.txt", "./data.json");

        // Or write a rendered template file
        env.write(temp, data, "./result.txt");
        env.write_with_json_file("./templates/greeting.txt", "./data.json", "./result.txt");
    }
    return 0;
}