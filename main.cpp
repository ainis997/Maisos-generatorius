#include <maisos_fjos.h>
#include <pagalb_fjos.h>
#include <string>
#include <iostream>

// #include <iostream>
// #include <string>
// #include <vector>
// #include <array>
// #include <cstdint>
// #include <cstring>
// #include <bit>
// #include <iomanip>

int main()
{
    for (;;)
    {
        std::string investis_str;
        std::cout << "Invesk tekstu:\n";
        std::getline(std::cin, investis_str);
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