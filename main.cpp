#include "maisos_fjos.h"
#include "pagalb_fjos.h"
#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <map>

struct StrPora
{
    std::string str1;
    std::string str2;
};

int main()
{
    istatyt_utf8(); // windows aplinkos terminalui padaro UTF-8 koduotę (Linux ir macOS ją ir taip naudoja)

    std::cout << "Maišos generavimo programa.\nNorėdami baigti programą, spauskite Ctrl+C.\n\n";
    for (;;)
    {
        std::string eiga;
        std::cout << "\nPasirinkite programos eigą:\n1 - Rankinis teksto įvedimas\n2 - Failo turinio įvestis\n3 - Efektyvumo matavimas (su failu)\n4 - Kolizijos tyrimas\nPasirinkimas: ";
        if (!std::getline(std::cin, eiga))
        {
            std::cout << "\n\nPrograma baigiama...";
            break;
        }
        if (eiga != "1" && eiga != "2" && eiga != "3" && eiga != "4")
        {
            std::cout << "Tokio pasirinkimo nėra.\n";
            continue;
        }

        // vvvvv ĮVESTIES RUOŠIMAS vvvvv

        std::string investis_str;
        if (eiga == "1")
        {
            std::cout << "\nPasirinktas teksto įvesties būdas: RANKINIS\n\nĮveskite tekstą:\n";
            if (!std::getline(std::cin, investis_str))
            {
                std::cout << "\n\nPrograma baigiama...";
                break;
            }
            if (investis_str.length() == 0)
                continue;
        }
        else if (eiga == "2" || eiga == "3") // 2 - failo įvestis arba 3 - matavimai (vis tiek failo reik)
        {
            std::cout << "\nPasirinktas teksto įvesties būdas: FAILAS\n\nPasirinkite failą pasirinkimo lange...\n";
            std::string failo_kelias;
            try
            {
                failo_kelias = failo_pasirinkimas(); // failo_pasirinkimas() meta runtime_error, jeigu nepavyko failo pasirinkt
                std::ifstream failas(failo_kelias);
                if (!failas.is_open())
                {
                    throw std::runtime_error("Nepavyko nuskaityti failo.");
                }
                std::cout << "Pasirinktas failas: " << failo_kelias << "\n\n";
                std::stringstream buferis;
                buferis << failas.rdbuf();
                investis_str = buferis.str();
                if (investis_str.size() == 0)
                {
                    throw std::runtime_error("Pasirinktas failas yra tuščias.");
                }
                failas.close();
            }
            catch (const std::runtime_error &e)
            {
                std::cerr << e.what() << '\n';
                continue;
            }
        }

        // spausd_baitais(investis); // JEIGU DIDELIS FAILAS, TAI ŠAKĖS
        std::cout << '\n';

        // vvvvv DOROJIMAS vvvvv

        if (eiga == "3")
        {
            // rast eilučių galus ('\n' pozicijas/indeksus)
            std::vector<size_t> eiluciu_galai; // eiluciuGalai[k] parodys, kiek baitų sudaro pirmos k eilučių (0..k). Realiai tai eiluciuGalai[k] laikys eilutės k+1 pradinio baito INDEKSĄ.
            size_t pos = 0;
            while (pos < investis_str.size())
            {
                size_t nauj_eil = investis_str.find('\n', pos); // ras indeksą artimiausio simbolio '\n', ieškant nuo indekso pos
                if (nauj_eil == std::string::npos)              // jeigu simbolio \n neberanda (== nebėr daugiau eilučių), tai .find() grąžina std::string::npos
                {
                    eiluciu_galai.push_back(investis_str.size());
                    break;
                }
                eiluciu_galai.push_back(nauj_eil + 1); // '\n' įeina eilutėn, tai eilutės paskutinis simbolis
                pos = nauj_eil + 1;
            }

            // pats algoritmas
            size_t eiliu_sk = eiluciu_galai.size();
            size_t hashuotinu_eiliu_sk = 1; // bus 1, 2, 4, 8, ..., o paskutinėj iteracijoj iš tikro hashuosim ne šį sk. eilių, o kiek faktiškai jų yra (tam atskirt naudosim std::min())
            for (;;)
            {
                size_t hashuojamu_eiliu_sk = std::min(hashuotinu_eiliu_sk, eiliu_sk);
                size_t paskutinio_baito_idx = eiluciu_galai.at(hashuojamu_eiliu_sk - 1); // -1, nes indeksavimas nuo 0 gi
                std::cout << "\nIštraukos eilių sk.: " << std::dec << hashuojamu_eiliu_sk << "\nIštraukos baitų sk.: " << paskutinio_baito_idx + 1 << '\n';
                std::vector<std::uint8_t> eiluciu_gabalas(investis_str.begin(), investis_str.begin() + paskutinio_baito_idx);
                std::vector<std::uint32_t> blokai = baitus_sujungt_po_4(std::move(eiluciu_gabalas));
                auto pr = std::chrono::high_resolution_clock::now();
                std::vector<std::uint8_t> hashas = gaut_256bit_hasha(std::move(blokai)); // efektyviau taip negu daryt atskirą tarpinį kintamąjį baitus_sujungt_po_4(investis) išvesčiai
                auto pab = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> laikas = pab - pr;
                std::cout << "Maišos trukmė (s): " << laikas.count() << "\nMaiša: "; // laikas sekundėm, double tipo; ČIA vis dar bus std::hex, bet tai neveikia laikas.count(), nes tai double!
                spausd_baitais(hashas);

                if (hashuojamu_eiliu_sk == eiliu_sk)
                    break;
                hashuotinu_eiliu_sk *= 2;
            }
        }
        else if (eiga == "1" || eiga == "2")
        {
            std::vector<std::uint8_t> investis(investis_str.begin(), investis_str.end());
            auto inv_ilgis = investis.size();
            std::vector<std::uint32_t> blokai = baitus_sujungt_po_4(std::move(investis));
            std::vector<std::uint8_t> hashas = gaut_256bit_hasha(std::move(blokai));

            std::cout << "Maiša:\n";
            spausd_baitais(hashas);
            // spausd_raidem(hashas);
        }

        // vvvv KOLIZIJOS TYRIMO KODAS BENDRAI vvvv
        if (eiga == "4")
        {
            // ASCII charai 32-126 (visi kiti yra sisteminiai simboliai ir pan., nepanaudojami)
            const std::string raidynas =
                " !\"#$%&'()*+,-./"
                "0123456789"
                ":;<=>?@"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "[\\]^_`"
                "abcdefghijklmnopqrstuvwxyz"
                "{|}~";

            unsigned int sekla = 123456789;
            std::mt19937_64 generatorius(sekla);
            std::uniform_int_distribution<std::size_t> pasiskirstymas(0, raidynas.size() - 1); // duos tolygiai paskirstytą random indeksą raidyno simboliui parinkt
            std::cout << "\nEilučių generatorius: std::mt19937_64\nSėkla: " << sekla << '\n';

            size_t poruSk = 100000;

            std::vector<size_t> strDydziai = {10, 100, 500, 1000};
            //  std::map<size_t, std::vector<StrPora>> strPoruVektoriai;

            for (size_t strDydis : strDydziai)
            {
                std::vector<StrPora> kolizijos;
                for (size_t i = 0; i < poruSk; ++i)
                {
                    std::string str1 = generuotRandomStr(10, raidynas, generatorius, pasiskirstymas);
                    std::string str2 = generuotRandomStr(10, raidynas, generatorius, pasiskirstymas);
                    while (str1 == str2) // beveik neįmanoma, bet atsargai
                    {
                        str2 = generuotRandomStr(10, raidynas, generatorius, pasiskirstymas);
                    }
                    auto hash1 = str_i_hasha(str1);
                    auto hash2 = str_i_hasha(str2);
                    if (hash1 == hash2)
                    {
                        kolizijos.push_back({str1, str2});
                    }
                }
                std::cout << "Tarp " << poruSk << " eilučių (" << strDydis << " ilgio) porų, rasta " << kolizijos.size() << " kolizijų.\n";
                if (kolizijos.size() > 0)
                {
                    std::cout << "Kolizijos:\n";
                    for (StrPora kolizija : kolizijos)
                    {
                        std::cout << kolizija.str1 << " : " << kolizija.str2 << '\n';
                    }
                }
            }
        }
    }
    return 0;
}