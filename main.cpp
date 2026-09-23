#include "maisos_fjos.h"
#include "pagalb_fjos.h"
#include <string>
#include <iostream>

int main()
{
    istatyt_utf8(); // windows aplinkos terminalui padaro UTF-8 koduotę (Linux ir macOS ją ir taip naudoja)

    std::cout << "Maišos generavimo programa.\nNorėdami baigti programą, spauskite Ctrl+C.\n\n";
    for (;;)
    {
        std::string investis_str;
        std::cout << "\nĮveskite tekstą:\n";
        if (!std::getline(std::cin, investis_str))
        {
            std::cout << "\nPrograma baigiama...";
            break;
        }
        if (investis_str.length() == 0)
            continue;
        std::vector<std::uint8_t> investis(investis_str.begin(), investis_str.end());
        auto inv_ilgis = investis.size();
        spausd_baitais(investis);
        std::cout << '\n';

        std::vector<std::uint32_t> blokai = baitus_sujungt_po_4(investis);
        std::vector<std::uint8_t> hashas = gaut_256bit_hasha(blokai);

        spausd_baitais(hashas);
        spausd_raidem(hashas);
    }
    return 0;
}