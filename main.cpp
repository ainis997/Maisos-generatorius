#include <iostream>
#include <string>
#include <vector>
#include <array>
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
        std::cout << baitas;
    }
    std::cout << '\n';
}

int main()
{
    std::string investis_str;
    std::cout << "Invesk tekstu:\n";
    std::getline(std::cin, investis_str);
    std::vector<uint8_t> investis(investis_str.begin(), investis_str.end());
    auto inv_ilgis = investis.size();
    spausd_baitais(investis);
    std::cout << '\n';
    // ======================================================
    // ===== skaidymas 32 bitų blokuos

    size_t paddingas = (4 - (inv_ilgis % 4)) % 4; // tam, kad "investis" ilgis būtų 4-ių kartotinis
    investis.resize(inv_ilgis + paddingas);
    std::vector<uint32_t> blokai;
    // reiktų rezervuot iš anksto vektoriui vietą
    for (size_t i = 0; i < investis.size(); i += 4) // investis.size() visada turėtų būti 4-ių kartotinis (dėl resize'o)
    {
        uint32_t blokas;
        std::memcpy(&blokas, &investis[i], 4); // 4, nes 32 / 8 = 4 (bloką sudarys 4 baitai)
        blokas = std::byteswap(blokas);
        // std::cout << ":: " << blokas << '\n';
        blokai.push_back(blokas);
    }
    // =======================================================
    // ===== MAIŠA

    for (auto it = blokai.begin(); it < blokai.end(); ++it)
    {
        auto idx = std::distance(blokai.begin(), it);
        *it += idx;
        *it *= 7;
        *it = std::rotr(*it, idx % 3 + 1);
    }

    for (auto it = blokai.begin(); it < blokai.end() - 1; ++it)
    {
        *it = *it * *(it + 1);
        *it = *it ^ *(it + 1);
    }
    for (auto it = blokai.rbegin(); it < blokai.rend() - 1; ++it)
    {
        *it = *it ^ *(it + 1);
    }
    // =====

    // =======================================================
    // ===== MAIŠA (256 bitų bloko paruošimas)

    if (blokai.size() < 8)
    {
        int idx = blokai.size() - 1;
        while (blokai.size() < 8)
        {
            if (idx < 0)
                idx = blokai.size() - 1;
            uint32_t naujas_blokas = blokai.at(idx);
            naujas_blokas = std::rotr(naujas_blokas, idx % 2 + 1);
            --idx;
        }
    }
    else if (blokai.size() > 8)
    {
        //
    }
    else
    {
        //
    }

    // =======================================================
    // ===== blokų atskaidymas baituos

    std::vector<uint8_t> isvestis;
    for (auto blokas : blokai)
    {
        std::array<uint8_t, 4> baitai4;
        std::memcpy(baitai4.data(), &blokas, sizeof(blokas));
        for (auto it = baitai4.rbegin(); it < baitai4.rend(); ++it) // nes baitus atvirkščiai dėlioja kompe
        {
            isvestis.push_back(*it);
        }
    }
    // =====

    spausd_baitais(isvestis);
    spausd_raidem(isvestis);

    return 0;
}