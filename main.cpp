#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <bit>

const int ISVESTIES_BAITU_SK = 32;

void spausd_baitais(std::vector<uint8_t> baitai)
{
    for (uint8_t baitas : baitai)
    {
        std::cout << std::hex << static_cast<int>(baitas) << " ";
    }
    std::cout << '\n';
}

void spausd_raidem(std::vector<uint8_t> baitai)
{
    for (uint8_t baitas : baitai)
    {
        std::cout << baitas << " ";
    }
    std::cout << '\n';
}

int main()
{
    std::string investis_str = "Ląbas rįţas wisēimans!";
    std::vector<uint8_t> investis(investis_str.begin(), investis_str.end());
    auto inv_ilgis = investis.size();
    spausd_baitais(investis);

    // ===== skaidymas blokuos
    size_t paddingas = 4 - (inv_ilgis % 4); // tam, kad "investis" ilgis būtų 4-ių kartotinis
    investis.resize(inv_ilgis + paddingas);
    std::cout << '\n'
              << investis.size() << '\n';

    std::vector<uint32_t> blokai;
    // reiktų rezervuot iš anksto vektoriui vietą
    for (size_t i = 0; i < investis.size(); i += 4) // investis.size() visada turėtų būti 4-ių kartotinis (dėl resize'o)
    {
        uint32_t blokas;
        std::memcpy(&blokas, &investis[i], 4); // 4, nes 32 / 8 = 4 (bloką sudarys 4 baitai)
        blokas = std::byteswap(blokas);
        std::cout << blokas << '\n';
        blokai.push_back(blokas);
    }

    spausd_baitais(investis);
    spausd_raidem(investis);

    return 0;
}