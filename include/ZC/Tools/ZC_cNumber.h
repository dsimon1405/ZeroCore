#pragma once

#include <ZC/ZC_Types.h>

#include <concepts>

template <typename T>
concept ZC_cNumber = std::same_as<T, sch_zc>
                || std::same_as<T, uch_zc>
                || std::same_as<T, sh_zc>
                || std::same_as<T, ush_zc>
                || std::same_as<T, i_zc>
                || std::same_as<T, ui_zc>
                || std::same_as<T, l_zc>
                || std::same_as<T, ul_zc>
                || std::same_as<T, f_zc>
                || std::same_as<T, d_zc>
                || std::same_as<T, ld_zc>;