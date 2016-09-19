#pragma once

#include "labels/label.h"

#include <glm/glm.hpp>

namespace Tangram {

class TextLabels;
class TextStyle;

struct GlyphQuad {
    size_t atlas;
    struct {
        glm::i16vec2 pos;
        glm::u16vec2 uv;
    } quad[4];
};

using TextRange = std::array<Range, 3>;

struct TextVertex {
    glm::i16vec2 pos;
    glm::u16vec2 uv;
    struct State {
        uint32_t selection;
        uint32_t color;
        uint32_t stroke;
        uint16_t alpha;
        uint16_t scale;
    } state;

    const static float position_scale;
    const static float alpha_scale;
};

class TextLabel : public Label {

public:

    struct VertexAttributes {
        uint32_t fill;
        uint32_t stroke;
        uint8_t fontScale;
        uint32_t selectionColor;
    };

    TextLabel(Label::WorldTransform _transform, Type _type, Label::Options _options,
              TextLabel::VertexAttributes _attrib,
              glm::vec2 _dim, TextLabels& _labels, TextRange _textRanges,
              TextLabelProperty::Align _preferedAlignment,
              size_t _anchorPoint = 0, const std::vector<glm::vec2>& _line = {});

    LabelType renderType() const override { return LabelType::text; }

    TextRange& textRanges() {
        return m_textRanges;
    }


    void obbs(ScreenTransform& _transform, std::vector<OBB>& _obbs,
              Range& _range, bool _append) override;


protected:

    void addVerticesToMesh(ScreenTransform& _transform) override;

    uint32_t selectionColor() override {
        return m_fontAttrib.selectionColor;
    }

private:

    void applyAnchor(LabelProperty::Anchor _anchor) override;

    bool updateScreenTransform(const glm::mat4& _mvp, const ViewState& _viewState,
                               ScreenTransform& _transform, bool _drawAllLabels) override;

    // Back-pointer to owning container
    const TextLabels& m_textLabels;

    // first vertex and count in m_textLabels quads (left,right,center)
    TextRange m_textRanges;

    // TextRange currently used for drawing
    int m_textRangeIndex;

    VertexAttributes m_fontAttrib;

    // The text LAbel prefered alignment
    TextLabelProperty::Align m_preferedAlignment;

    size_t m_anchorPoint;
    std::vector<glm::vec2> m_line;

};

}
