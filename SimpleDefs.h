#pragma once

#include "Mask.h"
#include "Symmetry.h"

namespace NPalagyi {
    extern const Mask TemplatesU[6];
    bool IsDeletable( int bits, EFaceDir dir );
    void GenerateHeader( const char* filename );
}

namespace NRaynal {
    extern const Mask TemplatesU[7];
    bool IsDeletable( int bits, EFaceDir dir );
    void GenerateHeader( const char* filename );
}

namespace NIsthmus {// Raynal
    extern const Mask TemplatesU[6];
    bool IsIsthmus( int bits );
    void GenerateHeader( const char* filename );

    namespace NDevelop {
        void Veryfy_IsIsthmus( const std::vector<std::uint8_t>& isthmus );
        void Verify_Templates( const std::vector<std::uint8_t>& primary, const std::vector<std::uint8_t>& isthmus );

        inline bool IsIsthmusByDefinition( int bits, const std::vector<std::uint8_t>& table ) {
            return (NComponent::GetForeCount( table[bits] ) >= 2);
        }
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
    int CalcP2xU( int bits );
    void CheckConditionsU( std::vector<std::uint8_t>& pxcond );

    namespace NDevelop {
        void Verify_CalcP2xU();
    }

    // P2x-simple
    extern const Mask TemplatesT26RxU[2];
    bool IsDeletable( int bits, EFaceDir dir );
    void GenerateHeader( const char* filename );

    namespace NDevelop {
        bool IsP2xSimpleU( int bits );
        void Verify_IsP2xSimpleU( const std::vector<std::uint8_t>& pxcond );
    }
}

namespace NNemeth {
    extern const Mask TemplatesU[3];
    extern const Mask TemplatesUN[1];
    extern const Mask TemplatesUNW[1];
    bool IsDeletable( int bits, int idx );  // idx = 0(U/N/W), 1(D/S/E)
    void GenerateHeader( const char* filename );
}
