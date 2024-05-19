/***************************************************************************
 *   Copyright (C) 2019 PCSX-Redux authors                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.           *
 ***************************************************************************/

#include "core/system.h"
#include "imgui.h"
#include "spu/interface.h"

struct Grid {
    static constexpr auto FlagsColumn = ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed;

    static constexpr auto FlagsRow = ImGuiTableRowFlags_None;

    static constexpr auto FlagsTable =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

    static constexpr auto FlagsTableOuter = FlagsTable | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollX;
    static constexpr auto FlagsTableInner = ImGuiTableFlags_Borders;

    static constexpr auto WidthGeneralIndex = 22.0f;
    static constexpr auto WidthGeneralOn = 30.0f;
    static constexpr auto WidthGeneralTag = 120.0f;
    static constexpr auto WidthGeneralOff = 30.0f;
    static constexpr auto WidthGeneralMute = 45.0f;
    static constexpr auto WidthGeneralSolo = 45.0f;
    static constexpr auto WidthGeneralNoise = 70.0f;
    static constexpr auto WidthGeneralFMod = 70.0f;
    static constexpr auto WidthGeneralPlot = 150.0f;
    static constexpr auto WidthGeneral =
        WidthGeneralIndex +
        WidthGeneralTag +
        WidthGeneralOn +
        WidthGeneralOff +
        WidthGeneralMute +
        WidthGeneralSolo +
        WidthGeneralNoise +
        WidthGeneralFMod +
        WidthGeneralPlot;

    static constexpr auto WidthFrequencyActive = 70.0f;
    static constexpr auto WidthFrequencyUsed = 70.0f;
    static constexpr auto WidthFrequency =
        WidthFrequencyActive +
        WidthFrequencyUsed;

    static constexpr auto WidthPositionStart = 120.0f;
    static constexpr auto WidthPositionCurrent = 120.0f;
    static constexpr auto WidthPositionLoop = 120.0f;
    static constexpr auto WidthPosition =
        WidthPositionStart +
        WidthPositionCurrent +
        WidthPositionLoop;

    static constexpr auto WidthVolumeL = 70.0f;
    static constexpr auto WidthVolumeR = 70.0f;
    static constexpr auto WidthVolume =
        WidthVolumeL +
        WidthVolumeR;

    static constexpr auto WidthAdsrA = 70.0f;
    static constexpr auto WidthAdsrD = 70.0f;
    static constexpr auto WidthAdsrS = 70.0f;
    static constexpr auto WidthAdsrR = 70.0f;
    static constexpr auto WidthAdsr =
        WidthAdsrA +
        WidthAdsrD +
        WidthAdsrS +
        WidthAdsrR;

    static constexpr auto WidthAdsrSustainLevel = 70.0f;
    static constexpr auto WidthAdsrSustainIncrease = 70.0f;
    static constexpr auto WidthAdsrSustain =
        WidthAdsrSustainLevel +
        WidthAdsrSustainIncrease;

    static constexpr auto WidthAdsrVolumeCurrent = 70.0f;
    static constexpr auto WidthAdsrVolumeEnvelope = 80.0f;
    static constexpr auto WidthAdsrVolume =
        WidthAdsrVolumeCurrent +
        WidthAdsrVolumeEnvelope;

    static constexpr auto WidthReverbAllowed = 70.0f;
    static constexpr auto WidthReverbActive = 70.0f;
    static constexpr auto WidthReverbNumber = 70.0f;
    static constexpr auto WidthReverbOffset = 70.0f;
    static constexpr auto WidthReverbRepeat = 70.0f;
    static constexpr auto WidthReverb =
        WidthReverbAllowed +
        WidthReverbActive +
        WidthReverbNumber +
        WidthReverbOffset +
        WidthReverbRepeat;
};

using namespace PCSX::SPU;
using SPU_CHANNELS = SPUCHAN(&)[impl::MAXCHAN + 1];

using SPU_CHANNELS_TAGS = char (&)[impl::MAXCHAN][impl::CHANNEL_TAG];
using SPU_CHANNELS_PLOT = float (&)[impl::MAXCHAN][impl::DEBUG_SAMPLES];
void DrawTable(SPU_CHANNELS& channels, size_t channelsCount, const float rowHeight) {
}

void DrawTableGeneralIndex(const unsigned channel)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text("%02i", channel);
}

void DrawTableGeneralTag(const unsigned channel, SPU_CHANNELS_TAGS &tags)
{
    ImGui::PushItemWidth(Grid::WidthGeneralTag);
    const auto tagLabel = "##SpuChannelTag" + std::to_string(channel);
    const auto tagHint = "Channel " + std::to_string(channel);
    ImGui::InputTextWithHint(tagLabel.c_str(), tagHint.c_str(), tags[channel], impl::CHANNEL_TAG);
    ImGui::PopItemWidth();
}

void DrawTableGeneralOn(const Chan::Data& data)
{
    ImGui::BeginDisabled();
    auto bit1 = data.get<Chan::On>().value;
    ImGui::Checkbox("", &bit1);
    ImGui::EndDisabled();
}

void DrawTableGeneralOff(const Chan::Data& data)
{
    auto bit2 = data.get<Chan::Stop>().value;
    ImGui::BeginDisabled();
    ImGui::Checkbox("", &bit2);
    ImGui::EndDisabled();
}

void DrawTableGeneralMute(SPU_CHANNELS channels, size_t channelsCount, const std::string ch, const ImVec2 buttonSize, const ImVec4 buttonTint, bool& muteThis, bool& soloThis, const ImGuiStyle style)
{
    ImGui::PushStyleColor(ImGuiCol_Button, muteThis ? ImVec4(0.6f, 0.0f, 0.0f, 1.0f) : buttonTint);
    std::string muteLabel = "M##SpuMute" + ch;
    const auto muteSize = ImVec2(
        (Grid::WidthGeneralMute - buttonSize.x) * 0.5f - style.FramePadding.x * 2.0f, 0);
    ImGui::Dummy(muteSize);
    ImGui::SameLine();
    if (ImGui::Button(muteLabel.c_str(), buttonSize)) {
        muteThis = !muteThis;
        if (muteThis) {
            soloThis = false;
        }
        if (ImGui::GetIO().KeyShift) {
            std::for_each(channels, channels + channelsCount, [muteThis](SPUCHAN& c) {
                c.data.get<Chan::Mute>().value = muteThis;
                if (muteThis) {
                    c.data.get<Chan::Solo>().value = false;
                }
            });
        }
    }
    ImGui::PopStyleColor();
}

void DrawTableGeneralSolo(SPU_CHANNELS channels, size_t channelsCount, unsigned i, const std::string ch, const ImVec2 buttonSize, const ImVec4 buttonTint, bool& muteThis, bool& soloThis, const ImGuiStyle style)
{
    ImGui::PushStyleColor(ImGuiCol_Button, soloThis ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : buttonTint);
    std::string soloLabel = "S##SpuSolo" + ch;
    const auto soloSize = ImVec2(
        (Grid::WidthGeneralSolo - buttonSize.x) * 0.5f - style.FramePadding.x * 2.0f, 0);
    ImGui::Dummy(soloSize);
    ImGui::SameLine();
    if (ImGui::Button(soloLabel.c_str(), buttonSize)) {
        soloThis = !soloThis;
        if (soloThis) {
            muteThis = false;
        }
        for (unsigned j = 0; j < channelsCount; j++) {
            if (j == i) {
                continue;
            }
            auto& dataOther = channels[j].data;
            auto& muteOther = dataOther.get<Chan::Mute>().value;
            auto& soloOther = dataOther.get<Chan::Solo>().value;
            if (soloThis) {
                // multi/single solo
                if (ImGui::GetIO().KeyShift) {
                    if (soloOther == false) {
                        muteOther = true;
                    }
                } else {
                    muteOther = true;
                    soloOther = false;
                }
            } else {
                // mute this to keep solo ones correct
                if (std::ranges::any_of(channels, channels + channelsCount, [](const SPUCHAN& c) {
                    return c.data.get<Chan::Solo>().value;
                })) {
                    muteThis = true;
                }
            }
        }

        // no more solo channels -> ensure none are muted
        if (std::ranges::all_of(channels, [](const SPUCHAN& c) {
            return c.data.get<Chan::Solo>().value == false;
        })) {
            std::for_each(channels, channels + channelsCount, [](SPUCHAN& c) {
                c.data.get<Chan::Mute>().value = false;
            });
        }
    }
    ImGui::PopStyleColor();
}

void DrawTableGeneralNoise(const Chan::Data& data)
{
    ImGui::Text("%i", data.get<Chan::Noise>().value);
}

void DrawTableGeneralFMod(const Chan::Data& data)
{
    ImGui::Text("%i", data.get<Chan::FMod>().value);
}

void DrawTableGeneralPlot(SPU_CHANNELS_PLOT plot, float padding, unsigned i)
{
    const auto plotSize = ImVec2(Grid::WidthGeneralPlot - padding, 0);
    ImGui::PlotHistogram("", plot[i], impl::DEBUG_SAMPLES, 0, nullptr, 0.0f, 1.0f, plotSize);
}

void DrawTableGeneral(
    SPU_CHANNELS& channels,
    size_t channelsCount,
    const float rowHeight,
    SPU_CHANNELS_TAGS& tags,
    SPU_CHANNELS_PLOT& plot,
    float padding) {
    if (ImGui::BeginTable("TableGeneral", 9, Grid::FlagsTableInner)) {
        ImGui::TableSetupColumn("#", Grid::FlagsColumn, Grid::WidthGeneralIndex);
        ImGui::TableSetupColumn("Tag", Grid::FlagsColumn, Grid::WidthGeneralTag);
        ImGui::TableSetupColumn("On", Grid::FlagsColumn, Grid::WidthGeneralOn);
        ImGui::TableSetupColumn("Off", Grid::FlagsColumn, Grid::WidthGeneralOff);
        ImGui::TableSetupColumn("Mute", Grid::FlagsColumn, Grid::WidthGeneralMute);
        ImGui::TableSetupColumn("Solo", Grid::FlagsColumn, Grid::WidthGeneralSolo);
        ImGui::TableSetupColumn("Noise", Grid::FlagsColumn, Grid::WidthGeneralNoise);
        ImGui::TableSetupColumn("FMod", Grid::FlagsColumn, Grid::WidthGeneralFMod);
        ImGui::TableSetupColumn("Plot", Grid::FlagsColumn, Grid::WidthGeneralPlot);

        ImGui::TableHeadersRow();
        for (auto i = 0u; i < channelsCount; ++i) {
            const auto& data = channels[i].data;

            ImGui::TableNextRow(Grid::FlagsRow, rowHeight);
            // @formatter:off
            ImGui::TableNextColumn(); DrawTableGeneralIndex(i);
            ImGui::TableNextColumn(); DrawTableGeneralTag(i, tags);
            ImGui::TableNextColumn(); DrawTableGeneralOn(data);
            ImGui::TableNextColumn(); DrawTableGeneralOff(data);
            // @formatter:on
            const auto ch = std::to_string(i);
            const auto buttonSize = ImVec2(rowHeight, 0);
            const auto buttonTint = ImGui::GetStyleColorVec4(ImGuiCol_Button);
            auto& dataThis = channels[i].data;
            auto& muteThis = dataThis.get<Chan::Mute>().value;
            auto& soloThis = dataThis.get<Chan::Solo>().value;

            const auto style = ImGui::GetStyle();
            // @formatter:off
            ImGui::TableNextColumn(); DrawTableGeneralMute(channels, channelsCount, ch, buttonSize, buttonTint, muteThis, soloThis, style);
            ImGui::TableNextColumn(); DrawTableGeneralSolo(channels, channelsCount, i, ch, buttonSize, buttonTint, muteThis, soloThis, style);
            ImGui::TableNextColumn(); DrawTableGeneralNoise(data);
            ImGui::TableNextColumn(); DrawTableGeneralFMod(data);
            ImGui::TableNextColumn(); DrawTableGeneralPlot(plot, padding, i);
            // @formatter:on
        }
        ImGui::EndTable();
    }
}

void DrawTableFrequency(SPU_CHANNELS& channels, size_t channelsCount, const float rowHeight) {
    if (ImGui::BeginTable("TableFrequency", 2, Grid::FlagsTableInner)) {
        ImGui::TableSetupColumn("Active", Grid::FlagsColumn, Grid::WidthFrequencyActive);
        ImGui::TableSetupColumn("Used", Grid::FlagsColumn, Grid::WidthFrequencyUsed);
        ImGui::TableHeadersRow();
        for (auto i = 0u; i < channelsCount; ++i) {
            const auto& data = channels[i].data;
            ImGui::TableNextRow(Grid::FlagsRow, rowHeight);
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%i", data.get<Chan::ActFreq>().value);
            ImGui::TableNextColumn();
            ImGui::Text("%i", data.get<Chan::UsedFreq>().value);
        }
        ImGui::EndTable();
    }
}

void DrawTablePosition(SPU_CHANNELS& channels, size_t channelsCount, const float rowHeight, const uint8_t* spuMemC) {
    if (ImGui::BeginTable("TablePosition", 3, Grid::FlagsTableInner)) {
        ImGui::TableSetupColumn("Start", Grid::FlagsColumn, Grid::WidthPositionStart);
        ImGui::TableSetupColumn("Current", Grid::FlagsColumn, Grid::WidthPositionCurrent);
        ImGui::TableSetupColumn("Loop", Grid::FlagsColumn, Grid::WidthPositionLoop);
        ImGui::TableHeadersRow();
        for (auto i = 0u; i < channelsCount; ++i) {
            const auto& chan = channels[i];
            ImGui::TableNextRow(Grid::FlagsRow, rowHeight);
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%i", static_cast<int>(chan.pStart - spuMemC));
            ImGui::TableNextColumn();
            ImGui::Text("%i", static_cast<int>(chan.pCurr - spuMemC));
            ImGui::TableNextColumn();
            ImGui::Text("%i", static_cast<int>(chan.pLoop - spuMemC));
        }
        ImGui::EndTable();
    }
}

void DrawTableVolume(SPU_CHANNELS& channels, size_t channelsCount, const float rowHeight) {
    if (ImGui::BeginTable("TableVolume", 2, Grid::FlagsTableInner)) {
        ImGui::TableSetupColumn("L", Grid::FlagsColumn, Grid::WidthVolumeL);
        ImGui::TableSetupColumn("R", Grid::FlagsColumn, Grid::WidthVolumeR);
        ImGui::TableHeadersRow();
        for (auto i = 0u; i < channelsCount; ++i) {
            const auto& data = channels[i].data;
            ImGui::TableNextRow(Grid::FlagsRow, rowHeight);
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%i", data.get<Chan::LeftVolume>().value);
            ImGui::TableNextColumn();
            ImGui::Text("%i", data.get<Chan::RightVolume>().value);
        }
        ImGui::EndTable();
    }
}

void DrawTableAdsr(SPU_CHANNELS& channels, size_t channelsCount, const float rowHeight) {
    if (ImGui::BeginTable("TableAdsr", 4, Grid::FlagsTableInner)) {
        ImGui::TableSetupColumn("A", Grid::FlagsColumn, Grid::WidthAdsrA);
        ImGui::TableSetupColumn("D", Grid::FlagsColumn, Grid::WidthAdsrD);
        ImGui::TableSetupColumn("S", Grid::FlagsColumn, Grid::WidthAdsrS);
        ImGui::TableSetupColumn("R", Grid::FlagsColumn, Grid::WidthAdsrR);
        ImGui::TableHeadersRow();
        for (auto i = 0u; i < channelsCount; ++i) {
            const auto& data = channels[i].ADSRX;
            ImGui::TableNextRow(Grid::FlagsRow, rowHeight);
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%i", data.get<exAttackRate>().value ^ 0x7F);
            ImGui::TableNextColumn();
            ImGui::Text("%i", (data.get<exDecayRate>().value ^ 0x1F) / 4);
            ImGui::TableNextColumn();
            ImGui::Text("%i", data.get<exSustainRate>().value ^ 0x7F);
            ImGui::TableNextColumn();
            ImGui::Text("%i", (data.get<exReleaseRate>().value ^ 0x1F) / 4);
        }
        ImGui::EndTable();
    }
}

void DrawTableAdsrSustain(SPU_CHANNELS& channels, size_t channelsCount, const float rowHeight) {
    if (ImGui::BeginTable("TableAdsrSustain", 2, Grid::FlagsTableInner)) {
        ImGui::TableSetupColumn("Level", Grid::FlagsColumn, Grid::WidthAdsrSustainLevel);
        ImGui::TableSetupColumn("Increase", Grid::FlagsColumn, Grid::WidthAdsrSustainIncrease);
        ImGui::TableHeadersRow();
        for (auto i = 0u; i < channelsCount; ++i) {
            const auto& data = channels[i].ADSRX;
            ImGui::TableNextRow(Grid::FlagsRow, rowHeight);
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%i", data.get<exSustainLevel>().value >> 27);
            ImGui::TableNextColumn();
            ImGui::Text("%i", data.get<exSustainIncrease>().value);
        }
        ImGui::EndTable();
    }
}

void DrawTableAdsrVolume(SPU_CHANNELS& channels, size_t channelsCount, const float rowHeight) {
    if (ImGui::BeginTable("TableAdsrVolume", 2, Grid::FlagsTableInner)) {
        ImGui::TableSetupColumn("Current", Grid::FlagsColumn, Grid::WidthAdsrVolumeCurrent);
        ImGui::TableSetupColumn("Envelope", Grid::FlagsColumn, Grid::WidthAdsrVolumeEnvelope);
        ImGui::TableHeadersRow();
        for (auto i = 0u; i < channelsCount; ++i) {
            const auto& data = channels[i].ADSRX;
            ImGui::TableNextRow(Grid::FlagsRow, rowHeight);
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%i", data.get<exVolume>().value);
            ImGui::TableNextColumn();
            ImGui::Text("%08X", data.get<exEnvelopeVol>().value);
        }
        ImGui::EndTable();
    }
}

void DrawTableReverb(SPU_CHANNELS& channels, size_t channelsCount, const float rowHeight) {
    if (ImGui::BeginTable("TableReverb", 5, Grid::FlagsTableInner)) {
        ImGui::TableSetupColumn("Allowed", Grid::FlagsColumn, Grid::WidthReverbAllowed);
        ImGui::TableSetupColumn("Active", Grid::FlagsColumn, Grid::WidthReverbActive);
        ImGui::TableSetupColumn("Number", Grid::FlagsColumn, Grid::WidthReverbNumber);
        ImGui::TableSetupColumn("Offset", Grid::FlagsColumn, Grid::WidthReverbOffset);
        ImGui::TableSetupColumn("Repeat", Grid::FlagsColumn, Grid::WidthReverbRepeat);
        ImGui::TableHeadersRow();
        for (auto i = 0u; i < channelsCount; ++i) {
            const auto& data = channels[i].data;
            ImGui::TableNextRow(Grid::FlagsRow, rowHeight);
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%i", data.get<Chan::Reverb>().value);
            ImGui::TableNextColumn();
            ImGui::Text("%i", data.get<Chan::RVBActive>().value);
            ImGui::TableNextColumn();
            ImGui::Text("%i", data.get<Chan::RVBNum>().value);
            ImGui::TableNextColumn();
            ImGui::Text("%i", data.get<Chan::RVBOffset>().value);
            ImGui::TableNextColumn();
            ImGui::Text("%i", data.get<Chan::RVBRepeat>().value);
        }
        ImGui::EndTable();
    }
}

void PCSX::SPU::impl::debug() {
    auto delta = std::chrono::steady_clock::now() - m_lastUpdated;
    using namespace std::chrono_literals;
    while (delta >= 50ms) {
        m_lastUpdated += 50ms;
        delta -= 50ms;
        for (unsigned ch = 0; ch < MAXCHAN; ch++) {
            if (!s_chan[ch].data.get<Chan::On>().value) {
                m_channelDebugTypes[ch][m_currentDebugSample] = EMPTY;
                m_channelDebugData[ch][m_currentDebugSample] = 0.0f;
            };
            if (s_chan[ch].data.get<Chan::IrqDone>().value) {
                m_channelDebugTypes[ch][m_currentDebugSample] = IRQ;
                m_channelDebugData[ch][m_currentDebugSample] = 0.0f;
                s_chan[ch].data.get<Chan::IrqDone>().value = 0;
                continue;
            }

            if (s_chan[ch].data.get<Chan::Mute>().value) {
                m_channelDebugTypes[ch][m_currentDebugSample] = MUTED;
            } else if (s_chan[ch].data.get<Chan::Noise>().value) {
                m_channelDebugTypes[ch][m_currentDebugSample] = NOISE;
            } else if (s_chan[ch].data.get<Chan::FMod>().value == 1) {
                m_channelDebugTypes[ch][m_currentDebugSample] = FMOD1;
            } else if (s_chan[ch].data.get<Chan::FMod>().value == 2) {
                m_channelDebugTypes[ch][m_currentDebugSample] = FMOD2;
            } else {
                m_channelDebugTypes[ch][m_currentDebugSample] = DATA;
            }

            m_channelDebugData[ch][m_currentDebugSample] =
                fabsf((float)s_chan[ch].data.get<Chan::sval>().value / 32768.0f);
        }
        if (++m_currentDebugSample == DEBUG_SAMPLES) m_currentDebugSample = 0;
    }
    if (!m_showDebug) return;
    ImGui::SetNextWindowPos(ImVec2(20, 40), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(1200, 430), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(_("SPU Debug"), &m_showDebug)) {
        ImGui::End();
        return;
    }

    {
        constexpr auto simpleTableFlags = ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_NoHostExtendX;
        constexpr auto simpleTableWidth = 150;

        if (ImGui::CollapsingHeader("SPU", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginTable("SpuBase", 4, simpleTableFlags)) {
                ImGui::TableSetupColumn("IRQ", 0, simpleTableWidth);
                ImGui::TableSetupColumn("CTRL", 0, simpleTableWidth);
                ImGui::TableSetupColumn("STAT", 0, simpleTableWidth);
                ImGui::TableSetupColumn("MEM", 0, simpleTableWidth);
                ImGui::TableHeadersRow();
                // @formatter:off
                ImGui::TableNextColumn(); ImGui::Text("%08X", static_cast<long>(pSpuIrq ? -1 : pSpuIrq - spuMemC));
                ImGui::TableNextColumn(); ImGui::Text("%04X", spuCtrl);
                ImGui::TableNextColumn(); ImGui::Text("%04X", spuStat);
                ImGui::TableNextColumn(); ImGui::Text("%i", spuAddr);
                // @formatter:on
                ImGui::EndTable();
            }
        }
        if (ImGui::CollapsingHeader("XA", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginTable("SpuXa", 5, simpleTableFlags)) {
                ImGui::TableSetupColumn("Frequency", 0, simpleTableWidth);
                ImGui::TableSetupColumn("Stereo", 0, simpleTableWidth);
                ImGui::TableSetupColumn("Samples", 0, simpleTableWidth);
                ImGui::TableSetupColumn("Volume L", 0, simpleTableWidth);
                ImGui::TableSetupColumn("Volume R", 0, simpleTableWidth);
                ImGui::TableHeadersRow();
                // @formatter:off
                ImGui::TableNextColumn(); ImGui::Text("%i", xapGlobal ? xapGlobal->freq : 0);
                ImGui::TableNextColumn(); ImGui::Text("%i", xapGlobal ? xapGlobal->stereo : 0);
                ImGui::TableNextColumn(); ImGui::Text("%i", xapGlobal ? xapGlobal->nsamples : 0);
                ImGui::TableNextColumn(); ImGui::Text("%i", iLeftXAVol);
                ImGui::TableNextColumn(); ImGui::Text("%i", iRightXAVol);
                // @formatter:on
                ImGui::EndTable();
            }
        }
    }

    if (ImGui::CollapsingHeader("Channels", ImGuiTreeNodeFlags_DefaultOpen)) {
        const auto style = ImGui::GetStyle();
        const auto rowHeight = ImGui::GetFrameHeightWithSpacing();
        const auto headerHeight = ImGui::GetTextLineHeightWithSpacing();
        const auto tableHeight = rowHeight * MAXCHAN + headerHeight * 2 + 4 + style.ScrollbarSize;

        // BUG ImGui hides last column border when scrolling (off by 1px)
        if (ImGui::BeginTable("SpuChannels", 8, Grid::FlagsTableOuter, ImVec2(0, tableHeight))) {
            constexpr auto fix = 2; // BUG ImGui may screw up last column width
            constexpr auto pad = 18;
            ImGui::TableSetupColumn("General", Grid::FlagsColumn, Grid::WidthGeneral + pad * fix);
            ImGui::TableSetupColumn("Frequency", Grid::FlagsColumn, Grid::WidthFrequency + pad);
            ImGui::TableSetupColumn("Position", Grid::FlagsColumn, Grid::WidthPosition + pad);
            ImGui::TableSetupColumn("Volume", Grid::FlagsColumn, Grid::WidthVolume + pad);
            ImGui::TableSetupColumn("ADSR", Grid::FlagsColumn, Grid::WidthAdsr + pad * fix);
            ImGui::TableSetupColumn("ADSR Sustain", Grid::FlagsColumn, Grid::WidthAdsrSustain + pad);
            ImGui::TableSetupColumn("ADSR Volume", Grid::FlagsColumn, Grid::WidthAdsrVolume + pad);
            ImGui::TableSetupColumn("Reverb", Grid::FlagsColumn, Grid::WidthReverb + pad * fix);
            ImGui::TableHeadersRow();

            ImGui::TableNextColumn();
            DrawTableGeneral(s_chan, MAXCHAN, rowHeight, m_channelTag, m_channelDebugData, pad);

            ImGui::TableNextColumn();
            DrawTableFrequency(s_chan, MAXCHAN, rowHeight);

            ImGui::TableNextColumn();
            DrawTablePosition(s_chan, MAXCHAN, rowHeight, spuMemC);

            ImGui::TableNextColumn();
            DrawTableVolume(s_chan, MAXCHAN, rowHeight);

            ImGui::TableNextColumn();
            DrawTableAdsr(s_chan, MAXCHAN, rowHeight);

            ImGui::TableNextColumn();
            DrawTableAdsrSustain(s_chan, MAXCHAN, rowHeight);

            ImGui::TableNextColumn();
            DrawTableAdsrVolume(s_chan, MAXCHAN, rowHeight);

            ImGui::TableNextColumn();
            DrawTableReverb(s_chan, MAXCHAN, rowHeight);
            ImGui::EndTable();
        }
    }

    ImGui::End();
}
