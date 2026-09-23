#include <pagalb_fjos.h>
#include <iostream>
#include <iomanip>

void spausd_baitais(std::vector<uint8_t> baitai)
{
    std::cout << std::hex; // tai lieka kaip nustatymas
    for (uint8_t baitas : baitai)
    {
        std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(baitas) << " ";
    }
    std::cout << '\n';
}

void spausd_raidem(std::vector<uint8_t> baitai)
{
    for (uint8_t baitas : baitai)
    {
        std::cout << baitas;
    }
    std::cout << '\n';
}

// std::vector<uint8_t> gaut_ivesties_baitus(std::string ivestis)
// {
//     return;
// }

std::vector<uint32_t> baitus_sujungt_po_4(std::vector<uint8_t> baitai)
{
    auto inv_ilgis = baitai.size();
    size_t paddingas = (4 - (inv_ilgis % 4)) % 4; // tam, kad vektoriaus "baitai" ilgis būtų 4-ių kartotinis
    baitai.resize(inv_ilgis + paddingas);
    std::vector<uint32_t> blokai;
    // reiktų rezervuot iš anksto vektoriui vietą
    for (size_t i = 0; i < baitai.size(); i += 4) // investis.size() visada turėtų būti 4-ių kartotinis (dėl resize'o)
    {
        uint32_t blokas;
        std::memcpy(&blokas, &baitai[i], 4); // 4, nes 32 / 8 = 4 (bloką sudarys 4 baitai)
        blokas = std::byteswap(blokas);
        // std::cout << ":: " << blokas << '\n';
        blokai.push_back(blokas);
    }
    return blokai;
}