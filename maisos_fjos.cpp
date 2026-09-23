#include <maisos_fjos.h>
#include <bit> // dėl std::rotr (rotate bits to right)
#include <array>

std::vector<std::uint8_t> gaut_256bit_hasha(std::vector<std::uint32_t> blokai)
{
    // ===== blokų skaičiaus suvienodinimas (8 4-baičiai blokai)

    if (blokai.size() < 8)
    {
        int idx = blokai.size() - 1;
        while (blokai.size() < 8)
        {
            if (idx < 0)
                idx = blokai.size() - 1;
            std::uint32_t naujas_blokas = blokai.at(idx);
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

    std::vector<std::uint8_t> isvestis;
    for (auto blokas : blokai)
    {
        std::array<std::uint8_t, 4> baitai4;
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