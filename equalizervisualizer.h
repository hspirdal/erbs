#ifndef EQUALIZERVISUALIZER_H
#define EQUALIZERVISUALIZER_H

#include <gmParametricsModule>

using namespace GMlib;

class EqualizerVisualizer : public PSurfVisualizer<float,3>
{
  GM_VISUALIZER(EqualizerVisualizer)
public:
  EqualizerVisualizer();
  EqualizerVisualizer( const EqualizerVisualizer& copy );

  void          render( const DisplayObject* obj, const Camera* cam ) const;
  void          renderGeometry( const GL::Program& prog, const DisplayObject* obj, const Camera* cam ) const;

  virtual void  replot( const DMatrix< DMatrix< Vector<float, 3> > >& p,
                        const DMatrix< Vector<float, 3> >& normals,
                        int m1, int m2, int d1, int d2,
                        bool closed_u, bool closed_v
  );

private:
  GL::Program                 _prog;

  GL::VertexBufferObject      _vbo;
  GL::IndexBufferObject       _ibo;
  GL::UniformBufferObject     _lights_ubo;
  GL::Texture                 _nmap;

  GLuint                      _no_strips;
  GLuint                      _no_strip_indices;
  GLsizei                     _strip_size;

  void                        draw() const;

  void                        initShaderProgram();
};

#endif // EQUALIZERVISUALIZER_H
