#pragma once
#include <vector>
#include <cstdint>
#include <string>

void spausd_baitais(std::vector<std::uint8_t> baitai);

void spausd_raidem(std::vector<std::uint8_t> baitai);

std::vector<std::uint32_t> baitus_sujungt_po_4(std::vector<std::uint8_t> baitai);

// ==========

void istatyt_utf8();

std::string failo_pasirinkimas() noexcept(false);