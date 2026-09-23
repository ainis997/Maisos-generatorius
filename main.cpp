#include "maisos_fjos.h"
#include "pagalb_fjos.h"
#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>

int main()
{
    istatyt_utf8(); // windows aplinkos terminalui padaro UTF-8 koduotę (Linux ir macOS ją ir taip naudoja)

    std::cout << "Maišos generavimo programa.\nNorėdami baigti programą, spauskite Ctrl+C.\n\n";
    for (;;)
    {
        std::string eiga;
        std::cout << "\nPasirinkite įvesties būdą:\n1 - Rankinis teksto įvedimas\n2 - Failo turinio įvestis\nPasirinkimas: ";
        if (!std::getline(std::cin, eiga))
        {
            std::cout << "\n\nPrograma baigiama...";
            break;
        }
        if (eiga != "1" && eiga != "2")
        {
            std::cout << "Tokio pasirinkimo nėra.\n";
            continue;
        }

        std::string investis_str;
        if (eiga == "1")
        {
            std::cout << "\nĮveskite tekstą:\n";
            if (!std::getline(std::cin, investis_str))
            {
                std::cout << "\n\nPrograma baigiama...";
                break;
            }
            if (investis_str.length() == 0)
                continue;
        }
        else // 2 - failo įvestis
        {
            std::string failo_kelias;
            try
            {
                failo_kelias = failo_pasirinkimas(); // failo_pasirinkimas() meta runtime_error, jeigu nepavyko failo pasirinkt
                std::ifstream failas(failo_kelias);
                if (!failas.is_open())
                {
                    throw std::runtime_error("Nepavyko nuskaityti failo.");
                }
                std::stringstream buferis;
                buferis << failas.rdbuf();
                investis_str = buferis.str();
                failas.close();
            }
            catch (const std::runtime_error &e)
            {
                std::cerr << e.what() << '\n';
                continue;
            }
        }

        std::vector<std::uint8_t> investis(investis_str.begin(), investis_str.end());
        auto inv_ilgis = investis.size();
        // spausd_baitais(investis); // JEIGU DIDELIS FAILAS, TAI ŠAKĖS
        std::cout << '\n';

        std::vector<std::uint32_t> blokai = baitus_sujungt_po_4(investis);
        std::vector<std::uint8_t> hashas = gaut_256bit_hasha(blokai);

        spausd_baitais(hashas);
        spausd_raidem(hashas);
    }
    return 0;
}