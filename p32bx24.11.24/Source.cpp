#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <iomanip> 
using namespace std;

string key = "454d8c83-c234-4846-a2cc-6c0b7317a018";

string d(double A) {
    
    string gio = to_string(A);

    for (auto& o : gio) {
        if (o == '.') {
            o = ',';
            break;
        }
    }
    return gio; 
}
void geocode_void(string parse, string places_r) {
    cpr::Response geocode = cpr::Get(cpr::Url{ parse });
    if (geocode.status_code != 200) {
        cout << "Ошибка при получении геокода: " << geocode.status_code << " - " << geocode.error.message << endl;
        return;
    }

    nlohmann::json json_geocode = nlohmann::json::parse(geocode.text);

    if (json_geocode.contains("result") && json_geocode["result"].contains("items") && !json_geocode["result"]["items"].empty()) {
        double lat = json_geocode["result"]["items"][0]["point"]["lat"];
        double lon = json_geocode["result"]["items"][0]["point"]["lon"];

        cout << fixed << setprecision(6) << lat << ' ' << lon << endl;


        places_r += d(lat);
        string rt = "," + d(lon);
        string r = places_r + rt;

        cpr::Response places = cpr::Get(cpr::Url{ r + "&key=" + key });
        if (places.status_code != 200) {
            cout << "Ошибка при получении мест: " << places.status_code << " - " << places.error.message << endl;
            return;
        }

        nlohmann::json json_places = nlohmann::json::parse(places.text);

        if (json_places.contains("result") && json_places["result"].contains("items") && !json_places["result"]["items"].empty()) {
            string address_comment = json_places["result"]["items"][0]["address_comment"];
            string address_name = json_places["result"]["items"][0]["address_name"];
            string name = json_places["result"]["items"][0]["name"];

            cout << address_comment << "\n" << address_name << "\n" << name << endl;
        }
        else {
            cout << "Нет найденных мест." << endl;
        }
    }
    else {
        cout << "Нет данных о геокоде." << endl;
    }


    }

void run() {
    while (true)
    {
        string parse;
        string parse2;
        string parse3;
        string parse4;
        string adris;
        string places_r_2;
        string places_r;
        string places_r_1;
        string setad;
        int user;
        cout << "1-найти но адресу: " << "2-выход";
        cin >> user;
        switch (user) {
        case 1:
            cout << "видите сначала адрес места Пример:Kазань,Баумана, 38/17;" << "\n" << "Видите  название места Пример: кафе;" << endl;
            cin >> adris >> setad;
            parse ="https ://catalog.api.2gis.com/3.0/items/geocode?q=";
            parse2 = parse + adris;
            parse3 = parse2 + "&fields=items.point&key=";
            parse4 =parse3 + key;
            places_r_2 = "https://catalog.api.2gis.com/3.0/items?q=";
            places_r_1 =setad + "&sort_point=";
            places_r = places_r_2 + places_r_1;
            geocode_void(parse4, places_r);
            break;

        case 2:
            return;

        }
    }
}
int main() {
    setlocale(LC_ALL, "Russian");
    run();
}