#include "pagalb_fjos.h"
#include <iostream>
#include <iomanip>
#include <cstring> // dėl std::memcpy()
#include <bit>     // dėl std::byteswap()
#include <exception>
#include "portable-file-dialogs.h"

#ifdef _WIN32 // jei programa veikia Windowsuos:
#include <windows.h>
#endif

void spausd_baitais(std::vector<std::uint8_t> baitai)
{
    std::cout << std::hex; // tai lieka kaip nustatymas
    for (std::uint8_t baitas : baitai)
    {
        std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(baitas) << " ";
    }
    std::cout << '\n';
}

void spausd_raidem(std::vector<std::uint8_t> baitai)
{
    for (std::uint8_t baitas : baitai)
    {
        std::cout << baitas;
    }
    std::cout << '\n';
}

std::vector<std::uint32_t> baitus_sujungt_po_4(std::vector<std::uint8_t> baitai)
{
    auto inv_ilgis = baitai.size();
    size_t paddingas = (4 - (inv_ilgis % 4)) % 4; // tam, kad vektoriaus "baitai" ilgis būtų 4-ių kartotinis
    baitai.resize(inv_ilgis + paddingas);
    std::vector<std::uint32_t> blokai;
    // reiktų rezervuot iš anksto vektoriui vietą
    for (size_t i = 0; i < baitai.size(); i += 4) // investis.size() visada turėtų būti 4-ių kartotinis (dėl resize'o)
    {
        std::uint32_t blokas;
        std::memcpy(&blokas, &baitai[i], 4); // 4, nes 32 / 8 = 4 (bloką sudarys 4 baitai)
        blokas = std::byteswap(blokas);
        // std::cout << ":: " << blokas << '\n';
        blokai.push_back(blokas);
    }
    return blokai;
}

// ==========

void istatyt_utf8()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

std::string failo_pasirinkimas() noexcept(false)
{
    auto pasirinkimas_vektoriuj = pfd::open_file(
                                      "Pasirinkite tekstinį failą",
                                      pfd::path::home(),
                                      {"Tekstiniai failai (.txt .text)", "*.txt *.text", "Visi failai", "*"})
                                      .result();
    if (pasirinkimas_vektoriuj.empty())
    {
        throw std::runtime_error("Nepavyko pasirinkti failo.");
    }
    return pasirinkimas_vektoriuj[0]; // neleidžiam rinktis kelių failų, tai tik vienas elementas bus vektoriuj
}