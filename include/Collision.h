#include <cmath>
#include <algorithm>

#include "Rect.h"

/**
	\brief Classe header-only que implementa lógica de colisão.

	Classe header-only com todos os métodos inline com objetivo de facilitar otimizações por parte do compilador.
*/
class Collision {
	public:
		/**
			\brief Verifica colisão.
			\return Verdadeiro se estiverem colidindo, falso caso contrário.

			Verifica colisão entre dois Rects levando em consideração seus ângulos.
			Observação: IsColliding espera ângulos em radianos!
		*/
		static inline bool IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB) {
			Vec2 A[] = { Vec2( a.x, a.y + a.h ),
						 Vec2( a.x + a.w, a.y + a.h ),
						 Vec2( a.x + a.w, a.y ),
						 Vec2( a.x, a.y )
					};
			Vec2 B[] = { Vec2( b.x, b.y + b.h ),
						 Vec2( b.x + b.w, b.y + b.h ),
						 Vec2( b.x + b.w, b.y ),
						 Vec2( b.x, b.y )
					};
			for (auto& v : A) {
				v = Rotate(v - a.Center(), angleOfA) + a.Center();
			}
			for (auto& v : B) {
				v = Rotate(v - b.Center(), angleOfB) + b.Center();
			}
			Vec2 axes[] = { Norm(A[0] - A[1]), Norm(A[1] - A[2]), Norm(B[0] - B[1]), Norm(B[1] - B[2]) };

			for (auto& axis : axes) {
				float P[4];

				for (int i = 0; i < 4; ++i) P[i] = Dot(A[i], axis);

				float minA = *std::min_element(P, P + 4);
				float maxA = *std::max_element(P, P + 4);

				for (int i = 0; i < 4; ++i) P[i] = Dot(B[i], axis);

				float minB = *std::min_element(P, P + 4);
				float maxB = *std::max_element(P, P + 4);

				if (maxA < minB || minA > maxB)
					return false;
			}

			return true;
		}

	private:

		/**
			\brief Método interno para obter Magnetude.
			\todo Verificar se usamos o nosso no lugar.

			Obtém a magnetude do Vec2 passado como argumento.
		*/
		static inline float Mag(const Vec2& p) {
			return std::sqrt(p.x * p.x + p.y * p.y);
		}

		/**
			\brief Método interno para obter vetor normalizado.
			\todo Verificar se usamos o nosso no lugar.

			Obtém o vetor normalizado do Vec2 passado como argumento.
		*/
		static inline Vec2 Norm(const Vec2& p) {
			return p * (1.f / Mag(p));
		}

		/**
			\brief Método interno para obter o produto escalar dos dois vetores.

			Obtém o produto escalar dos Vec2 passados como argumento.
		*/
		static inline float Dot(const Vec2& a, const Vec2& b) {
			return a.x * b.x + a.y * b.y;
		}

		/**
			\brief Método interno para rotacionar vetor .
			\todo Verificar se usamos o nosso no lugar.

			Rotaciona o vetor do Vec2 passado como argumento em angle graus.
		*/
		static inline Vec2 Rotate(const Vec2& p, float angle) {
			float cs = std::cos(angle), sn = std::sin(angle);
			return Vec2 ( p.x * cs - p.y * sn, p.x * sn + p.y * cs );
		}
};

// Aqui estão três operadores que sua classe Vec2 deve precisar, se já não tiver. 
// Se sua classe tiver métodos para Mag, Norm, Dot e/ou Rotate, você pode substituir
// os usos desses métodos por usos dos seus, mas garanta que deem resultados corretos.

// Vec2 operator+(const Vec2& rhs) const {
//	return Vec2(x + rhs.x, y + rhs.y);
// }

// Vec2 operator-(const Vec2& rhs) const {
//	return Vec2(x - rhs.x, y - rhs.y);
// }

// Vec2 operator*(const float rhs) const {
//	return Vec2(x * rhs, y * rhs);
// }
