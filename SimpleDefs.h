#pragma once

#include "Mask.h"
#include "Symmetry.h"

namespace NPalagyi {
    extern const Mask Templates[6];
    bool IsDeletable( int bits, EDir6 dir );
}

namespace NRaynal {
    extern const Mask Templates[7];
    bool IsDeletable( int bits, EDir6 dir );
}

namespace NIsthmus {// Raynal
    extern const MaskZero Templates[6];
    bool IsIsthmus( int bits );
    void Veryfy_IsIsthmus( const std::vector<std::uint8_t>& isthmus );
    void VerifyTemplates( const std::vector<std::uint8_t>& primary, const std::vector<std::uint8_t>& isthmus );

    inline bool IsIsthmusByDefinition( int bits, const std::vector<std::uint8_t>& table ) {
        return (NComponent::GetForeCount( table[bits] ) >= 2);
    }
}

namespace NPxSimple {// Lohou
    // Conditions
    enum ELC {
        ELC_Simple = 0x01,
        ELC_P2     = 0x02,
        ELC_T26    = 0x04,
        ELC_T6     = 0x08,
        ELC_C26    = 0x10,
        ELC_C6     = 0x20,
    };

    extern const MaskZero TemplatesT26Rx[2];

    void CheckConditionsUDir( std::vector<std::uint8_t>& pxcond );
    bool IsP2xSimple( int bits, EDir6 dir );

    bool IsP2xSimpleU( int bits );
    void Verify_IsP2xSimpleU( const std::vector<std::uint8_t>& pxcond );
}
