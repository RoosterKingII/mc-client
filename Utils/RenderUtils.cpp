#include "RenderUtils.h"
#include "../SDK/GameData.h"
#include "../resource.h"
#include <glm/ext/matrix_transform.hpp>
#include "GuiInfo.h"

#include <fstream> // Para ofstream
#include <windows.h> // Para funciones de recursos de Windows
Vec2<float> GuiInfo::ScreenRes = { 0, 0 };
Vec2<float> GuiInfo::TrueScreenRes = { 0, 0 };

void RenderUtils::SetUp(ScreenView* screenView, MinecraftUIRenderContext* ctx) {
	//if (!init) {
		renderCtx = ctx;
		screenContext2D = renderCtx->screenContext;
		Tessellator2D = screenContext2D->getTessellator();
		colorHolder = screenContext2D->getColorHolder();
		if (uiMaterial == nullptr) uiMaterial = MaterialPtr::createMaterial(HashedString("ui_textured_and_glcolor"));
		if (blendMaterial == nullptr) blendMaterial = MaterialPtr::createMaterial(HashedString("fullscreen_cube_overlay_blend"));
		mcFont = ctx->clientInstance->minecraftGame->mcFont;
		init = true;
	//}
	deltaTime = screenView->deltaTime;

    // *** DESCOMENTAR y MODIFICAR para usar guiData->windowSize ***
    if (screenView != nullptr && ctx != nullptr && ctx->clientInstance != nullptr && ctx->clientInstance->guiData != nullptr) {
        GuiInfo::ScreenRes.x = ctx->clientInstance->guiData->windowSize.x;
        GuiInfo::ScreenRes.y = ctx->clientInstance->guiData->windowSize.y;

        logF("[RenderUtils::SetUp] - screenWidth from guiData->windowSize: %f", ctx->clientInstance->guiData->windowSize.x);
        logF("[RenderUtils::SetUp] - screenHeight from guiData->windowSize: %f", ctx->clientInstance->guiData->windowSize.y);
        logF("[RenderUtils::SetUp] - GuiInfo::ScreenRes updated to: x=%f, y=%f", GuiInfo::ScreenRes.x, GuiInfo::ScreenRes.y);
    } else {
        logF("[RenderUtils::SetUp] - WARNING: screenView or context or clientInstance->guiData is NULL! Cannot update GuiInfo::ScreenRes.");
        if (screenView == nullptr) logF("[RenderUtils::SetUp] - screenView is NULL");
        if (ctx == nullptr) logF("[RenderUtils::SetUp] - ctx is NULL");
        if (ctx != nullptr && ctx->clientInstance == nullptr) logF("[RenderUtils::SetUp] - ctx->clientInstance is NULL");
        if (ctx != nullptr && ctx->clientInstance != nullptr && ctx->clientInstance->guiData == nullptr) logF("[RenderUtils::SetUp] - ctx->clientInstance->guiData is NULL");
    }
}

void RenderUtils::setGameRenderContext(ScreenContext* screenContext) {
	screenContext3D = screenContext;
}

void RenderUtils::setColor(const MC_Color& color) {
	colorHolder[0] = color.r;
	colorHolder[1] = color.g;
	colorHolder[2] = color.b;
	colorHolder[3] = color.a;
	*reinterpret_cast<uint8_t*>(colorHolder + 4) = 1;
}

void RenderUtils::setColor(float r, float g, float b, float a) {
	colorHolder[0] = r;
	colorHolder[1] = g;
	colorHolder[2] = b;
	colorHolder[3] = a;
	*reinterpret_cast<uint8_t*>(colorHolder + 4) = 1;
}

Tessellator* RenderUtils::getTessellator3D() {
	if (screenContext3D == nullptr) return nullptr;
	return screenContext3D->getTessellator();
}

void RenderUtils::drawText(const Vec2<float>& textPos, std::string textStr, MC_Color color, float textSize, float alpha, bool shadow) {
	static uintptr_t caretMeasureData = 0xFFFFFFFF;

	float tPos[4];
	tPos[0] = textPos.x;
	tPos[1] = textPos.x;
	tPos[2] = textPos.y;
	tPos[3] = textPos.y;

	TextMeasureData textMeasure(textSize, shadow);
	renderCtx->drawText(mcFont, tPos, &textStr, &color, alpha, 0, &textMeasure, &caretMeasureData);
}
void RenderUtils::flushImage(MC_Color color, float alpha)
{
	static HashedString flushString = HashedString(0xA99285D21E94FC80, "ui_flush");
	renderCtx->flushImages(color, alpha, flushString);
}
void RenderUtils::renderImage(std::string filePath, Vec4<float> rectPosition, Vec2<float> uvPos, Vec2<float> uvSize,Type type)
{
	ResourceLocation location = ResourceLocation(filePath, type);
	TextureData* textureData = new TextureData();
	bool textureLoaded = false; // Inicializar a false

	// *** NUEVO CÓDIGO - Extraer imagen del recurso si el archivo no existe ***
	std::string fullFilePath = Utils::getClientPath() + filePath;
	std::ifstream fileTest(fullFilePath);
	if (!fileTest.good()) { // Si el archivo NO existe
		fileTest.close();
		// *** MODIFICADO FindResource para usar GetModuleHandle(NULL) ***
		HRSRC resourceInfo = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_BACKGROUND_IMAGE), L"RCDATA"); // Buscar el recurso - USANDO GetModuleHandle(NULL)
		if (resourceInfo) {
			HGLOBAL resourceHandle = LoadResource(NULL, resourceInfo); // Cargar el recurso
			if (resourceHandle) {
				void* resourceData = LockResource(resourceHandle); // Bloquear para acceso
				DWORD resourceSize = SizeofResource(NULL, resourceInfo); // Obtener el tamaño

				if (resourceData && resourceSize > 0) {
					std::ofstream outputFile(fullFilePath, std::ios::binary); // Abrir archivo para escritura binaria
					if (outputFile.is_open()) {
						outputFile.write(static_cast<const char*>(resourceData), resourceSize); // Escribir los datos del recurso al archivo
						outputFile.close();
						logF("[renderImage] - Image extracted from resource to: %s", fullFilePath.c_str());
					}
					else {
						logF("[renderImage] - ERROR: Failed to open file for resource extraction: %s", fullFilePath.c_str());
					}
					UnlockResource(resourceHandle); // Desbloquear el recurso
				}
				else {
					logF("[renderImage] - ERROR: Failed to load resource data.");
				}
				FreeResource(resourceHandle); // Liberar el recurso cargado
			}
			else {
				logF("[renderImage] - ERROR: Failed to load resource handle.");
			}
		}
		else {
			logF("[renderImage] - ERROR: Resource not found: IDR_BACKGROUND_IMAGE");
		}
	}
	else {
		fileTest.close();
	}
	// *** FIN DEL NUEVO CÓDIGO ***

	logF("[renderImage] - Starting to render image from path: %s", fullFilePath.c_str()); // Usar fullFilePath ahora
	logF("[renderImage] - Calling renderCtx->getTexture...");
	textureLoaded = renderCtx->getTexture(textureData, &location); // Cargar la textura DESPUÉS de la extracción (o si ya existía)
	logF("[renderImage] - renderCtx->getTexture returned. Result: %s", textureLoaded ? "SUCCESS" : "FAILURE");

	if (!textureLoaded) {
		logF("[renderImage] - ERROR: getTexture FAILED to load texture: %s", fullFilePath.c_str()); // Usar fullFilePath aquí
		logF("[renderImage] - Path was: %s", fullFilePath.c_str());
		return;
	}

	logF("[renderImage] - getTexture SUCCEEDED. Calling renderCtx->drawImage...");
	renderCtx->drawImage(textureData, Vec2<float>(rectPosition.x, rectPosition.y), Vec2<float>(rectPosition.z - rectPosition.x, rectPosition.w - rectPosition.y), uvPos, uvSize);
	logF("[renderImage] - renderCtx->drawImage call completed.");
}
float RenderUtils::getTextWidth(const std::string& textStr, float textSize) {
	TextHolder text(textStr);
	return renderCtx->getLineLength(mcFont, &text, textSize, false);
}

float RenderUtils::getFontHeight(float textSize) {
	return mcFont->getLineHeight() * textSize;
}
void RenderUtils::fillRectangle(Vec4<float> pos, MC_Color col, float alpha)
{
	setColor(col);
	drawQuad({ pos.x, pos.w }, { pos.z, pos.w }, { pos.z, pos.y }, { pos.x, pos.y });
}
void RenderUtils::drawQuad(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Vec2<float>& p4, const MC_Color& color) {
	setColor(color);
	Tessellator2D->begin(VertextFormat::QUAD, 4);

	Tessellator2D->vertex(p1.x, p1.y, 0);
	Tessellator2D->vertex(p2.x, p2.y, 0);
	Tessellator2D->vertex(p3.x, p3.y, 0);
	Tessellator2D->vertex(p4.x, p4.y, 0);

	Tessellator2D->renderMeshImmediately(screenContext2D, uiMaterial);
}

void RenderUtils::drawQuad(const Vec2<float>& p1, const Vec2<float>& p2, const Vec2<float>& p3, const Vec2<float>& p4) {
	Tessellator2D->begin(VertextFormat::QUAD, 4);

	Tessellator2D->vertex(p1.x, p1.y, 0);
	Tessellator2D->vertex(p2.x, p2.y, 0);
	Tessellator2D->vertex(p3.x, p3.y, 0);
	Tessellator2D->vertex(p4.x, p4.y, 0);

	Tessellator2D->renderMeshImmediately(screenContext2D, uiMaterial);
}

void RenderUtils::Flush() {
	renderCtx->flushText(0.f);
}

void RenderUtils::drawLine2D(const Vec2<float>& start, const Vec2<float>& end, const MC_Color& color, float lineWidth) {
	setColor(color);
	float modX = 0.f - (start.y - end.y);
	float modY = start.x - end.x;

	float len = sqrtf(modX * modX + modY * modY);

	modX /= len;
	modY /= len;
	modX *= lineWidth;
	modY *= lineWidth;

	Tessellator2D->begin(VertextFormat::TRIANGLE_STRIP);

	Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
	Tessellator2D->vertex(start.x - modX, start.y - modY, 0);
	Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

	Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
	Tessellator2D->vertex(end.x + modX, end.y + modY, 0);
	Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

	Tessellator2D->renderMeshImmediately(screenContext2D, uiMaterial);
}
void RenderUtils::draw2DBox(const AABB& aabb, UIColor color, UIColor lineColor, float lineWidth, bool fill, bool corners) {
	Vec3<float> lower = Vec3<float>(aabb.lower.x, aabb.lower.y, aabb.lower.z);
	Vec3<float> upper = Vec3<float>(aabb.upper.x, aabb.upper.y, aabb.upper.z);

	Vec3<float> worldPoints[8];
	worldPoints[0] = Vec3<float>(lower.x, lower.y, lower.z);
	worldPoints[1] = Vec3<float>(lower.x, lower.y, upper.z);
	worldPoints[2] = Vec3<float>(upper.x, lower.y, lower.z);
	worldPoints[3] = Vec3<float>(upper.x, lower.y, upper.z);
	worldPoints[4] = Vec3<float>(lower.x, upper.y, lower.z);
	worldPoints[5] = Vec3<float>(lower.x, upper.y, upper.z);
	worldPoints[6] = Vec3<float>(upper.x, upper.y, lower.z);
	worldPoints[7] = Vec3<float>(upper.x, upper.y, upper.z);

	std::vector<Vec2<float>> points;
	for (int i = 0; i < 8; i++) {
		Vec2<float> screen;
		if (worldToScreen(worldPoints[i], screen)) {
			points.push_back(screen);
		}
	}

	if (points.size() < 1) return;

	Vec4<float> resultRect = { points[0].x, points[0].y, points[0].x, points[0].y };
	for (const auto& point : points) {
		if (point.x < resultRect.x) resultRect.x = point.x;
		if (point.y < resultRect.y) resultRect.y = point.y;
		if (point.x > resultRect.z) resultRect.z = point.x;
		if (point.y > resultRect.w) resultRect.w = point.y;
	}

	if (fill) {
		fillRectangle(resultRect, MC_Color(color.r, color.g, color.b, color.a), color.a / 255.0f);
	}

	if (!corners) {
		// Draw regular rectangle
		Vec2<float> start(resultRect.x, resultRect.y);
		Vec2<float> end(resultRect.z, resultRect.w);

		// Draw the lines
		drawLine2D(Vec2<float>(start.x, start.y), Vec2<float>(end.x, start.y), MC_Color(lineColor.r, lineColor.g, lineColor.b, lineColor.a), lineWidth);
		drawLine2D(Vec2<float>(start.x, end.y), Vec2<float>(end.x, end.y), MC_Color(lineColor.r, lineColor.g, lineColor.b, lineColor.a), lineWidth);
		drawLine2D(Vec2<float>(start.x, start.y), Vec2<float>(start.x, end.y), MC_Color(lineColor.r, lineColor.g, lineColor.b, lineColor.a), lineWidth);
		drawLine2D(Vec2<float>(end.x, start.y), Vec2<float>(end.x, end.y), MC_Color(lineColor.r, lineColor.g, lineColor.b, lineColor.a), lineWidth);
	}
	else {
		float length = (resultRect.z - resultRect.x) / 4.0f;
		MC_Color lineCol(lineColor.r, lineColor.g, lineColor.b, lineColor.a);

		// Top left
		drawLine2D(Vec2<float>(resultRect.x, resultRect.y), Vec2<float>(resultRect.x + length, resultRect.y), lineCol, lineWidth);
		drawLine2D(Vec2<float>(resultRect.x, resultRect.y), Vec2<float>(resultRect.x, resultRect.y + length), lineCol, lineWidth);

		// Top right
		drawLine2D(Vec2<float>(resultRect.z, resultRect.y), Vec2<float>(resultRect.z - length, resultRect.y), lineCol, lineWidth);
		drawLine2D(Vec2<float>(resultRect.z, resultRect.y), Vec2<float>(resultRect.z, resultRect.y + length), lineCol, lineWidth);

		// Bottom left
		drawLine2D(Vec2<float>(resultRect.x, resultRect.w), Vec2<float>(resultRect.x + length, resultRect.w), lineCol, lineWidth);
		drawLine2D(Vec2<float>(resultRect.x, resultRect.w), Vec2<float>(resultRect.x, resultRect.w - length), lineCol, lineWidth);

		// Bottom right
		drawLine2D(Vec2<float>(resultRect.z, resultRect.w), Vec2<float>(resultRect.z - length, resultRect.w), lineCol, lineWidth);
		drawLine2D(Vec2<float>(resultRect.z, resultRect.w), Vec2<float>(resultRect.z, resultRect.w - length), lineCol, lineWidth);
	}
}

void RenderUtils::drawBox(const AABB& blockAABB, UIColor color, UIColor lineColor, float lineWidth, bool fill, bool outline) {
	if (mc.getClientInstance()->getLevelRenderer() == nullptr) return;

	Vec3<float> lower = blockAABB.lower;
	Vec3<float> upper = blockAABB.upper;

	Vec3<float> diff = upper.sub(lower);
	Vec3<float> vertices[8];
	vertices[0] = Vec3<float>(lower.x, lower.y, lower.z);
	vertices[1] = Vec3<float>(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = Vec3<float>(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = Vec3<float>(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = Vec3<float>(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = Vec3<float>(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = Vec3<float>(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = Vec3<float>(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	std::shared_ptr<glmatrixf> refdef = std::shared_ptr<glmatrixf>(mc.getClientInstance()->getbadrefdef()->correct());
	Vec3<float> origin = mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1;
	Vec2<float> fov = mc.getClientInstance()->getFov();
	Vec2<float> screenSize = mc.getClientInstance()->guiData->windowSize;

	if (fill) {
		// Convert the vertices to screen coordinates
		std::vector<Vec2<float>> screenCords;
		for (int i = 0; i < 8; i++) {
			Vec2<float> screen;
			if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
				screenCords.push_back(screen);
			}
		}

		// Return if there are less than four points to draw quads with
		if (screenCords.size() < 8) return;

		// Define the indices of the vertices to use for each quad face
		static std::vector<std::tuple<int, int, int, int>> faces = {
			{0, 1, 3, 2},  // Bottom face
			{4, 5, 7, 6},  // Top face
			{0, 1, 5, 4},  // Front face
			{2, 3, 7, 6},  // Back face
			{1, 3, 7, 5},  // Right face
			{0, 2, 6, 4}   // Left face
		};

		// Draw the quads to fill the box
		for (auto face : faces) {
			drawQuad(screenCords[std::get<0>(face)], screenCords[std::get<1>(face)], screenCords[std::get<2>(face)], screenCords[std::get<3>(face)], color.toMC_Color());
			drawQuad(screenCords[std::get<3>(face)], screenCords[std::get<2>(face)], screenCords[std::get<1>(face)], screenCords[std::get<0>(face)], color.toMC_Color());
		}
	}

	{
		// Convert the vertices to screen coordinates
		std::vector<std::tuple<int, Vec2<float>>> screenCords;
		for (int i = 0; i < 8; i++) {
			Vec2<float> screen;
			if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
				screenCords.emplace_back(outline ? (int)screenCords.size() : i, screen);
			}
		}

		// Return if there are less than two points to draw lines between
		if (screenCords.size() < 2) return;

		switch (outline) {
		case false: {
			// Draw lines between all pairs of vertices
			for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
				auto from = *it;
				auto fromOrig = vertices[std::get<0>(from)];

				for (auto to : screenCords) {
					auto toOrig = vertices[std::get<0>(to)];

					// Determine if the line should be drawn based on the relative positions of the vertices
					bool shouldDraw = false;
					// X direction
					shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x;
					// Y direction
					shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y;
					// Z direction
					shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z;

					if (shouldDraw) drawLine2D(std::get<1>(from), std::get<1>(to), lineColor.toMC_Color(), lineWidth);
				}
			}
			return;
			break;
		}
		case true: {
			// Find start vertex
			auto it = screenCords.begin();
			std::tuple<int, Vec2<float>> start = *it;
			it++;
			for (; it != screenCords.end(); it++) {
				auto cur = *it;
				if (std::get<1>(cur).x < std::get<1>(start).x) {
					start = cur;
				}
			}

			// Follow outer line
			std::vector<int> indices;

			auto current = start;
			indices.push_back(std::get<0>(current));
			Vec2<float> lastDir(0, -1);
			do {
				float smallestAngle = PI * 2;
				Vec2<float> smallestDir;
				std::tuple<int, Vec2<float>> smallestE;
				auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
				for (auto cur : screenCords) {
					if (std::get<0>(current) == std::get<0>(cur))
						continue;

					// angle between vecs
					Vec2<float> dir = Vec2<float>(std::get<1>(cur)).sub(std::get<1>(current));
					float angle = atan2(dir.y, dir.x) - lastDirAtan2;
					if (angle > PI) {
						angle -= 2 * PI;
					}
					else if (angle <= -PI) {
						angle += 2 * PI;
					}
					if (angle >= 0 && angle < smallestAngle) {
						smallestAngle = angle;
						smallestDir = dir;
						smallestE = cur;
					}
				}
				indices.push_back(std::get<0>(smallestE));
				lastDir = smallestDir;
				current = smallestE;
			} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

			// draw

			Vec2<float> lastVertex;
			bool hasLastVertex = false;
			for (auto& indice : indices) {
				Vec2<float> curVertex = std::get<1>(screenCords[indice]);
				if (!hasLastVertex) {
					hasLastVertex = true;
					lastVertex = curVertex;
					continue;
				}

			    drawLine2D(lastVertex, curVertex, lineColor.toMC_Color(), lineWidth);
				lastVertex = curVertex;
			}
			return;
			break;
		}
		}
	}
}

void RenderUtils::drawBox(const Vec3<float>& blockPos, UIColor color, UIColor lineColor, float lineWidth, bool fill, bool outline) {
	AABB blockAABB;
	blockAABB.lower = blockPos;
	blockAABB.upper.x = blockPos.x + 1.f;
	blockAABB.upper.y = blockPos.y + 1.f;
	blockAABB.upper.z = blockPos.z + 1.f;
	RenderUtils::drawBox(blockAABB, color, lineColor, lineWidth, fill, outline);
}
bool RenderUtils::DrawAABB(const AABB& aabb, ImU32 espCol, float thickness) {
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	std::shared_ptr<glmatrixf> refdef = std::shared_ptr<glmatrixf>(mc.getClientInstance()->getbadrefdef()->correct());
	Vec3<float> cornerA = aabb.lower;
	Vec3<float> cornerH = aabb.upper;
	Vec3<float> cornerB = { cornerH.x, cornerA.y, cornerA.z };
	Vec3<float> cornerC = { cornerA.x, cornerA.y, cornerH.z };
	Vec3<float> cornerD = { cornerH.x, cornerA.y, cornerH.z };
	Vec3<float> cornerE = { cornerA.x, cornerH.y, cornerA.z };
	Vec3<float> cornerF = { cornerH.x, cornerH.y, cornerA.z };
	Vec3<float> cornerG = { cornerA.x, cornerH.y, cornerH.z };

	Vec2<float> screenPosA = { 0,0 }; if (!RenderUtils::worldToScreen(cornerA, screenPosA)) { return false; }
	Vec2<float> screenPosB = { 0,0 }; if (!RenderUtils::worldToScreen(cornerB, screenPosB)) { return false; }
	Vec2<float> screenPosC = { 0,0 }; if (!RenderUtils::worldToScreen(cornerC, screenPosC)) { return false; }
	Vec2<float> screenPosD = { 0,0 }; if (!RenderUtils::worldToScreen(cornerD, screenPosD)) { return false; }
	Vec2<float> screenPosE = { 0,0 }; if (!RenderUtils::worldToScreen(cornerE, screenPosE)) { return false; }
	Vec2<float> screenPosF = { 0,0 }; if (!RenderUtils::worldToScreen(cornerF, screenPosF)) { return false; }
	Vec2<float> screenPosG = { 0,0 }; if (!RenderUtils::worldToScreen(cornerG, screenPosG)) { return false; }
	Vec2<float> screenPosH = { 0,0 }; if (!RenderUtils::worldToScreen(cornerH, screenPosH)) { return false; }

	//Bottom face
	drawList->AddLine(screenPosA.toImVec2(), screenPosB.toImVec2(), espCol, thickness);
	drawList->AddLine(screenPosB.toImVec2(), screenPosD.toImVec2(), espCol, thickness);
	drawList->AddLine(screenPosC.toImVec2(), screenPosD.toImVec2(), espCol, thickness);
	drawList->AddLine(screenPosC.toImVec2(), screenPosA.toImVec2(), espCol, thickness);

	//Top face
	drawList->AddLine(screenPosE.toImVec2(), screenPosF.toImVec2(), espCol, thickness);
	drawList->AddLine(screenPosF.toImVec2(), screenPosH.toImVec2(), espCol, thickness);
	drawList->AddLine(screenPosG.toImVec2(), screenPosH.toImVec2(), espCol, thickness);
	drawList->AddLine(screenPosG.toImVec2(), screenPosE.toImVec2(), espCol, thickness);

	//Corners to connect bottom and top
	drawList->AddLine(screenPosA.toImVec2(), screenPosE.toImVec2(), espCol, thickness);
	drawList->AddLine(screenPosB.toImVec2(), screenPosF.toImVec2(), espCol, thickness);
	drawList->AddLine(screenPosC.toImVec2(), screenPosG.toImVec2(), espCol, thickness);
	drawList->AddLine(screenPosD.toImVec2(), screenPosH.toImVec2(), espCol, thickness);

	return true;
}

bool RenderUtils::DrawBlock(Vec3<int>& blockPos, ImU32 color, float thickness)
{
	AABB box = {
		blockPos.toFloat(),
		blockPos.add(1.f).toFloat()
	};
	return RenderUtils::DrawAABB(box, color, thickness);
}

bool RenderUtils::worldToScreen(const Vec3<float>& worldPos, Vec2<float>& screenPos) {
	auto refdef = mc.getClientInstance()->getbadrefdef()->correct();
	Vec2<float> fov = mc.getClientInstance()->getFov();
	Vec2<float> screenSize = mc.getClientInstance()->guiData->windowSize;

	return refdef->OWorldToScreen(mc.getClientInstance()->getLevelRenderer()->levelRendererPlayer->cameraPos1, worldPos, screenPos, fov, screenSize);
}

