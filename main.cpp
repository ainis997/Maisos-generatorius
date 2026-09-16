#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <cstring>
#include <bit>
#include <iomanip>

const int ISVESTIES_BAITU_SK = 32;

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

std::vector<uint8_t> gaut_256bit_hasha(std::vector<uint32_t> blokai)
{
    // ===== blokų skaičiaus suvienodinimas (8 4-baičiai blokai)

    if (blokai.size() < 8)
    {
        int idx = blokai.size() - 1;
        while (blokai.size() < 8)
        {
            if (idx < 0)
                idx = blokai.size() - 1;
            uint32_t naujas_blokas = blokai.at(idx);
            naujas_blokas = std::rotr(naujas_blokas, idx % 2 + 1);
            blokai.push_back(naujas_blokas);
            --idx;
        }
    }
    else if (blokai.size() > 8)
    {
        for (int i = 0; i < 8; ++i)
        {
            int idx = i + 8;
            while (idx < blokai.size())
            {
                blokai.at(i) = blokai.at(i) ^ blokai.at(idx);
                idx += 8;
            }
            blokai.at(i) = std::rotr(blokai.at(i), idx % 2 + 1);
        }
        blokai.resize(8);
    }
    else
    {
        for (int i = 0; i < 8; ++i)
        {
            blokai.at(i) = std::rotr(blokai.at(i), i % 2 + 1);
        }
    }

    // ===== papildoma blokų maiša

    for (int i = 0; i < blokai.size(); ++i)
    {
        for (int j = 0; j < blokai.size(); ++j)
        {
            blokai[i] += blokai[j];
        }
    }

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

    // ===== paskutinė maiša atskirais baitais (dėl lavinos efekto)
    for (int i = 0; i < isvestis.size(); ++i)
    {
        for (int j = 0; j < isvestis.size(); ++j)
        {
            if (i != j)
            {
                isvestis.at(i) += isvestis.at(j);
            }
        }
    }

    return isvestis;
}

int main()
{
    for (;;)
    {
        std::string investis_str;
        std::cout << "Invesk tekstu:\n";
        std::getline(std::cin, investis_str);
        if (investis_str.length() == 0)
            continue;
        std::vector<uint8_t> investis(investis_str.begin(), investis_str.end());
        auto inv_ilgis = investis.size();
        spausd_baitais(investis);
        std::cout << '\n';

        std::vector<uint32_t> blokai = baitus_sujungt_po_4(investis);
        std::vector<uint8_t> hashas = gaut_256bit_hasha(blokai);

        spausd_baitais(hashas);
        spausd_raidem(hashas);
    }
    return 0;
}