#pragma once

// =============================================================================
//  Compile-time XOR String Encryption (C++17)
//  
//  Uso: xorstr_("MinhaString")  ou  xs("MinhaString")
//  
//  A string e criptografada em tempo de compilacao e so e decifrada
//  em runtime quando acessada. Isso dificulta a analise estatica
//  de strings no binario.
// =============================================================================

#include <utility>
#include <cstddef>
#include <string>

namespace xor_impl
{
    // Chave derivada do indice - cada char usa uma chave diferente
    constexpr char XorKey(size_t index) {
        // Chave base pseudo-aleatoria usando indices
        constexpr char keys[] = { 0x5A, 0x3C, 0x7E, 0x12, 0x4F, 0x8B, 0x2D, 0x61, 
                                   0x99, 0xA7, 0xC3, 0xF1, 0x0E, 0x6D, 0xB5, 0x23 };
        return keys[index % sizeof(keys)] ^ static_cast<char>(index * 0x1B + 0x42);
    }

    // Array criptografado em tempo de compilacao
    template <size_t N>
    class XorString
    {
    private:
        char m_encrypted[N];
        
        // Criptografa um caractere em tempo de compilacao
        constexpr char encrypt(char c, size_t index) const {
            return c ^ XorKey(index);
        }
        
        // Decripta um caractere em runtime
        char decrypt(char c, size_t index) const {
            return c ^ XorKey(index);
        }

    public:
        // Construtor constexpr - criptografa em tempo de compilacao
        template <size_t... I>
        constexpr XorString(const char* str, std::index_sequence<I...>)
            : m_encrypted{ encrypt(str[I], I)... }
        {}

        // Retorna string decriptada (cria copia temporaria)
        const char* c_str() const {
            // Buffer estatico por thread para evitar aloc dinamica
            thread_local static char decrypted[N];
            for (size_t i = 0; i < N; ++i) {
                decrypted[i] = decrypt(m_encrypted[i], i);
            }
            return decrypted;
        }
        
        // Conversao implicita para const char*
        operator const char*() const {
            return c_str();
        }
        
        // Conversao para std::string
        std::string str() const {
            return std::string(c_str());
        }
    };

    // Factory function para deduzir o tamanho automaticamente
    template <size_t N>
    constexpr auto make_xorstr(const char(&str)[N]) {
        return XorString<N>(str, std::make_index_sequence<N>{});
    }
}

// Macro principal para uso
#define xorstr_(s) (xor_impl::make_xorstr(s).c_str())
#define xs(s) xorstr_(s)

// Versao que retorna std::string
#define xorstr_s(s) (xor_impl::make_xorstr(s).str())
#define xs_s(s) xorstr_s(s)
