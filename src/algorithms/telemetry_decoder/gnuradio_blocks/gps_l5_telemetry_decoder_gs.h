/*!
 * \file gps_l5_telemetry_decoder_gs.h
 * \brief Interface of a CNAV message demodulator block
 * \author Antonio Ramos, 2017. antonio.ramos(at)cttc.es
 * -----------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2020  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * -----------------------------------------------------------------------------
 */

#ifndef GNSS_SDR_GPS_L5_TELEMETRY_DECODER_GS_H
#define GNSS_SDR_GPS_L5_TELEMETRY_DECODER_GS_H


#include "GPS_L5.h"                       // for GPS_L5I_NH_CODE_LENGTH
#include "gnss_satellite.h"               // for Gnss_Satellite
#include "gps_cnav_navigation_message.h"  // for Gps_CNAV_Navigation_Message
#include <boost/circular_buffer.hpp>
#include <gnuradio/block.h>
#include <gnuradio/types.h>  // for gr_vector_const_void_star
#include <cstdint>
#include <fstream>
#include <string>
#if GNURADIO_USES_STD_POINTERS
#include <memory>  // for std::shared_ptr
#else
#include <boost/shared_ptr.hpp>
#endif

extern "C"
{
#include "cnav_msg.h"
}


class gps_l5_telemetry_decoder_gs;

#if GNURADIO_USES_STD_POINTERS
using gps_l5_telemetry_decoder_gs_sptr = std::shared_ptr<gps_l5_telemetry_decoder_gs>;
#else
using gps_l5_telemetry_decoder_gs_sptr = boost::shared_ptr<gps_l5_telemetry_decoder_gs>;
#endif

gps_l5_telemetry_decoder_gs_sptr gps_l5_make_telemetry_decoder_gs(
    const Gnss_Satellite &satellite,
    bool dump);

/*!
 * \brief This class implements a GPS L5 Telemetry decoder
 *
 */
class gps_l5_telemetry_decoder_gs : public gr::block
{
public:
    ~gps_l5_telemetry_decoder_gs();
    void set_satellite(const Gnss_Satellite &satellite);  //!< Set satellite PRN
    void set_channel(int32_t channel);                    //!< Set receiver's channel
    void reset();
    int general_work(int noutput_items, gr_vector_int &ninput_items,
        gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);

private:
    friend gps_l5_telemetry_decoder_gs_sptr gps_l5_make_telemetry_decoder_gs(
        const Gnss_Satellite &satellite,
        bool dump);

    gps_l5_telemetry_decoder_gs(const Gnss_Satellite &satellite, bool dump);

    cnav_msg_decoder_t d_cnav_decoder{};

    Gnss_Satellite d_satellite;

    Gps_CNAV_Navigation_Message d_CNAV_Message;

    std::string d_dump_filename;
    std::ofstream d_dump_file;

    uint64_t d_sample_counter;
    uint64_t d_last_valid_preamble;

    int32_t d_channel;

    uint32_t d_TOW_at_current_symbol_ms;
    uint32_t d_TOW_at_Preamble_ms;
    uint32_t d_max_symbols_without_valid_frame;

    bool d_flag_PLL_180_deg_phase_locked;
    bool d_flag_valid_word;
    bool d_sent_tlm_failed_msg;
    bool d_dump;
};


#endif  // GNSS_SDR_GPS_L5_TELEMETRY_DECODER_GS_H
