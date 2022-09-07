#include "Country.h"

using namespace std;

Country::Country() {
    cities.emplace_back(MyVertex(0, "Gdynia", 54.5189, 18.5305));
    cities.emplace_back(MyVertex(1, "Slupsk", 54.4641, 17.0285));
    cities.emplace_back(MyVertex(2, "Gdansk", 54.352, 18.6466));
    cities.emplace_back(MyVertex(3, "Koszalin", 54.1943, 16.1715));
    cities.emplace_back(MyVertex(4, "Elblag", 54.1561, 19.4045));
    cities.emplace_back(MyVertex(5, "Olsztyn", 53.7784, 20.4801));
    cities.emplace_back(MyVertex(6, "Grudziadz", 53.4837, 18.7536));
    cities.emplace_back(MyVertex(7, "Szczecin", 53.4285, 14.5528));
    cities.emplace_back(MyVertex(8, "Pila", 53.151, 16.7382));
    cities.emplace_back(MyVertex(9, "Bialystok", 53.1325, 23.1688));

    cities.emplace_back(MyVertex(10, "Bydgoszcz", 53.1235, 18.0084));
    cities.emplace_back(MyVertex(11, "Torun", 53.0138, 18.5984));
    cities.emplace_back(MyVertex(12, "Gorzow Wielkopolski", 52.7325, 15.2369));
    cities.emplace_back(MyVertex(13, "Wloclawek", 52.6483, 19.0677));
    cities.emplace_back(MyVertex(14, "Plock", 52.5463, 19.7065));
    cities.emplace_back(MyVertex(15, "Poznan", 52.4064, 16.9252));
    cities.emplace_back(MyVertex(16, "Warszawa", 52.2297, 21.0122));
    cities.emplace_back(MyVertex(17, "Konin", 52.2230, 18.2511));
    cities.emplace_back(MyVertex(18, "Siedlce", 52.1676, 22.2902));
    cities.emplace_back(MyVertex(19, "Zielona Gora", 51.9356, 15.5062));

    cities.emplace_back(MyVertex(20, "Kalisz", 51.7673, 18.0853));
    cities.emplace_back(MyVertex(21, "Lodz", 51.7592, 19.456));
    cities.emplace_back(MyVertex(22, "Piotrkow Trybunalski", 51.4052, 19.703));
    cities.emplace_back(MyVertex(23, "Radom", 51.4027, 21.1471));
    cities.emplace_back(MyVertex(24, "Lublin", 51.2465, 22.5684));
    cities.emplace_back(MyVertex(25, "Legnica", 51.207, 16.1553));
    cities.emplace_back(MyVertex(26, "Wroclaw", 51.1079, 17.0385));
    cities.emplace_back(MyVertex(27, "Jelenia Gora", 50.9044, 15.7194));
    cities.emplace_back(MyVertex(28, "Kielce", 50.8661, 20.6286));
    cities.emplace_back(MyVertex(29, "Czestochowa", 50.8118, 19.1203));

    cities.emplace_back(MyVertex(30, "Walbrzych", 50.784, 16.2844));
    cities.emplace_back(MyVertex(31, "Opole", 50.6751, 17.9213));
    cities.emplace_back(MyVertex(32, "Bytom", 50.3484, 18.9157));
    cities.emplace_back(MyVertex(33, "Zabrze", 50.3249, 18.7857));
    cities.emplace_back(MyVertex(34, "Dabrowa Gornicza", 50.3217, 19.1949));
    cities.emplace_back(MyVertex(35, "Chorzow", 50.2975, 18.9546));
    cities.emplace_back(MyVertex(36, "Gliwice", 50.2945, 18.6714));
    cities.emplace_back(MyVertex(37, "Sosnowiec", 50.2863, 19.1041));
    cities.emplace_back(MyVertex(38, "Katowice", 50.2649, 19.0238));
    cities.emplace_back(MyVertex(39, "Ruda Slaska", 50.2558, 18.8556));

    cities.emplace_back(MyVertex(40, "Myslowice", 50.208, 19.1661));
    cities.emplace_back(MyVertex(41, "Jaworzno", 50.205, 19.2739));
    cities.emplace_back(MyVertex(42, "Tychy", 50.1218, 19.02));
    cities.emplace_back(MyVertex(43, "Rybnik", 50.1022, 18.5463));
    cities.emplace_back(MyVertex(44, "Krakow", 50.0647, 19.945));
    cities.emplace_back(MyVertex(45, "Rzeszow", 50.0412, 21.9991));
    cities.emplace_back(MyVertex(46, "Tarnow", 50.0121, 20.9858));
    cities.emplace_back(MyVertex(47, "Jastrzebie-Zdroj", 49.9454, 18.6101));
    cities.emplace_back(MyVertex(48, "Bielsko-Biala", 49.8224, 19.0584));
    cities.emplace_back(MyVertex(49, "Nowy Sacz", 49.6175, 20.7153));
}

const vector<MyVertex> &Country::getCities() const {
    return cities;
}