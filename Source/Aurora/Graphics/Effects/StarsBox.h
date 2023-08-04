#ifndef STARSBOX_H_
#define STARSBOX_H_

#include <Source/Aurora/Graphics/Vertex.h>
#include <Source/Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <Source/Aurora/Math/Frustum.h>

namespace Aurora
{
	namespace Graphics
	{
		class StarsBox
		{
		public:
			StarsBox();
			~StarsBox();

            void Build();
			void Update();
			void Render(float alpha);

		private:

			TexturesPSPVertex *boxVertices;
			TexturesPSPVertex *boxVertices2;
			TexturesPSPVertex *boxVertices3;
			TexturesPSPVertex *boxVertices4;
			TexturesPSPVertex *boxVertices5;

			float size;
		};
	}
}

#endif

