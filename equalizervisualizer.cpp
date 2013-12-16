#include "equalizervisualizer.h"


/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/


/*! \file gmpsurfdefaultvisualizer.c
 *
 *  Implementation of the PSurfDefaultVisualizer template class.
 */



// gmlib
#include <opengl/gmopengl.h>
#include <opengl/gmopenglmanager.h>
#include <scene/gmscene.h>
#include <scene/camera/gmcamera.h>
#include <scene/light/gmlight.h>
#include <scene/utils/gmmaterial.h>

// stl
#include <set>
#include <string>

#include <QDebug>

  EqualizerVisualizer::EqualizerVisualizer()
    : _no_strips(0), _no_strip_indices(0), _strip_size(0)
  {


    initShaderProgram();

    _vbo.create();
    _ibo.create();
    _lights_ubo.acquire("lights_ubo");
    _nmap.create(GL_TEXTURE_2D);
  }


  EqualizerVisualizer::EqualizerVisualizer(const EqualizerVisualizer& copy)
    : PSurfVisualizer(copy), _no_strips(0), _no_strip_indices(0), _strip_size(0)
  {

    initShaderProgram();

    _vbo.create();
    _ibo.create();
    _lights_ubo.acquire("lights_ubo");
    _nmap.create(GL_TEXTURE_2D);
  }

  void EqualizerVisualizer::render( const DisplayObject* obj, const Camera* cam ) const
  {

    const HqMatrix<float,3> &mvmat = obj->getModelViewMatrix(cam);
    const HqMatrix<float,3> &pmat = obj->getProjectionMatrix(cam);

    this->glSetDisplayMode();

    _prog.bind(); {

      // Model view and projection matrices
      _prog.setUniform( "u_mvmat", mvmat );
      _prog.setUniform( "u_mvpmat", pmat * mvmat );
      _prog.setUniform( "u_nmat", obj->getMatrix());

      // Lights
      _prog.setUniformBlockBinding( "Lights", _lights_ubo, 0 );

      // Material
      const Material &m = obj->getMaterial();
      _prog.setUniform( "u_mat_amb", m.getAmb() );
      //_prog.setUniform( "u_mat_dif", m.getDif() );
      _prog.setUniform( "u_mat_dif", GMlib::Color(0.0f, 0.0f, 0.0f, 255.0f) );
      _prog.setUniform( "u_mat_spc", m.getSpc() );
      _prog.setUniform( "u_mat_shi", m.getShininess() );

      // Normal map
      _prog.setUniform( "u_nmap", _nmap, (GLenum)GL_TEXTURE0, 0 );

      // Get vertex and texture attrib locations
      GL::AttributeLocation vert_loc = _prog.getAttributeLocation( "in_vertex" );
      GL::AttributeLocation tex_loc = _prog.getAttributeLocation( "in_tex" );

      // Bind and draw
      _vbo.bind();
      _vbo.enable( vert_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexTex2D), reinterpret_cast<const GLvoid *>(0x0) );
      _vbo.enable( tex_loc,  2, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexTex2D), reinterpret_cast<const GLvoid *>(3*sizeof(GLfloat)) );

      draw();

      _vbo.disable( vert_loc );
      _vbo.disable( tex_loc );
      _vbo.unbind();

    } _prog.unbind();
  }

  void EqualizerVisualizer::draw() const
  {

    _ibo.bind();
    for( unsigned int i = 0; i < _no_strips; ++i )
      _ibo.drawElements( GL_TRIANGLE_STRIP, _no_strip_indices, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(i * _strip_size) );
    _ibo.unbind();
  }

  inline
  void EqualizerVisualizer::replot
  (
    const DMatrix< DMatrix< Vector<float, 3> > >& p,
    const DMatrix< Vector<float, 3> >& normals,
    int /*m1*/, int /*m2*/, int /*d1*/, int /*d2*/,
    bool closed_u, bool closed_v
  ) {

    PSurfVisualizer::fillStandardVBO( _vbo, p );
    PSurfVisualizer::fillTriangleStripIBO( _ibo, p.getDim1(), p.getDim2(), _no_strips, _no_strip_indices, _strip_size );
    PSurfVisualizer::fillNMap( _nmap, normals, closed_u, closed_v );
  }

  inline
  void EqualizerVisualizer::renderGeometry( const GL::Program& prog, const DisplayObject* obj, const Camera* cam ) const
  {

    prog.setUniform( "u_mvpmat", obj->getModelViewProjectionMatrix(cam) );
    GL::AttributeLocation vertice_loc = prog.getAttributeLocation( "in_vertex" );

    _vbo.bind();
    _vbo.enable( vertice_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GL::GLVertexTex2D), reinterpret_cast<const GLvoid *>(0x0) );

    draw();

    _vbo.disable( vertice_loc );
    _vbo.unbind();
  }

  void EqualizerVisualizer::initShaderProgram()
  {

    const std::string prog_name    = "eq_vis_default_prog";
    if( _prog.acquire(prog_name) ) return;



    std::string vs_src;
    vs_src.append( GL::OpenGLManager::glslDefHeader150Source() );
    vs_src.append(
      "uniform mat4 u_mvmat, u_mvpmat, u_nmat;\n"
      "\n"
      "in vec4 in_vertex;\n"
      "in vec2 in_tex;\n"
      "\n"
      "out vec4 gl_Position;\n"
      "\n"
      "smooth out vec3 ex_pos;\n"
      "smooth out vec2 ex_tex;\n"
      "out vec4 colorpos;\n"
      "\n"
      "void main() {\n"
      "\n"
      "  vec4 v_pos = u_mvmat * in_vertex;\n"
      "  ex_pos = v_pos.xyz * v_pos.w;\n"
          "colorpos =u_nmat * in_vertex;"
      "\n"
      "  ex_tex = in_tex;\n"
      "\n"
      "  gl_Position = u_mvpmat * in_vertex;\n"
      "}\n"
    );

    std::string fs_src;
    fs_src.append( GL::OpenGLManager::glslDefHeader150Source() );
    fs_src.append( GL::OpenGLManager::glslStructMaterialSource() );
    fs_src.append( GL::OpenGLManager::glslUniformLightsSource() );
    fs_src.append( GL::OpenGLManager::glslFnSunlightSource() );
    fs_src.append( GL::OpenGLManager::glslFnPointlightSource() );
    fs_src.append( GL::OpenGLManager::glslFnSpotlightSource() );
    fs_src.append( GL::OpenGLManager::glslFnComputeLightingSource() );
    fs_src.append(
      "uniform sampler2D u_nmap;\n"
      "uniform mat4      u_mvmat;\n"
      "\n"
      "uniform vec4      u_mat_amb;\n"
      "uniform vec4      u_mat_dif;\n"
      "uniform vec4      u_mat_spc;\n"
      "uniform float     u_mat_shi;\n"
      "\n"
      "smooth in vec3    ex_pos;\n"
      "smooth in vec2    ex_tex;\n"
      "in vec4 colorpos;\n"
      "\n"
      "out vec4 gl_FragColor;\n"
      "\n"
      "void main() {\n"
      "\n"
      "  mat3 nmat = inverse( transpose( mat3( u_mvmat ) ) );\n"
      "  vec3 nmap_normal = texture( u_nmap, ex_tex.st).xyz;\n"
      "  vec3 normal = normalize( nmat * nmap_normal );\n"
      "\n"
      "  Material mat;\n"
      "  mat.ambient   = u_mat_amb;\n"
      "  mat.diffuse   = u_mat_dif;\n"
      "  mat.specular  = u_mat_spc;\n"
      "  mat.shininess = u_mat_shi;\n"
      "\n"
      "  vec4 light_color = vec4(0.0);\n"
      "float alpha = 0.8f;\n"
          "vec4 c;\n"
          "vec4 red = vec4(1.0, 0.0, 0.0, 1.0);\n"
          "vec4 green = vec4(0.0, 1.0, 0.0, 1.0);\n"
            "c = mix(red, green, 1-colorpos.y/10);\n"
          "gl_FragColor = (1-alpha)*computeLighting( mat, normal, ex_pos ) + alpha*c;\n"
      "\n"
      "}\n"
    );

    bool compile_ok, link_ok;

    GL::VertexShader vshader;
    vshader.create("equal_vis_vs");
    vshader.setPersistent(true);
    vshader.setSource(vs_src);
    compile_ok = vshader.compile();
    assert(compile_ok);

    GL::FragmentShader fshader;
    fshader.create("equal_vis_fs");
    fshader.setPersistent(true);
    fshader.setSource(fs_src);
    compile_ok = fshader.compile();
    assert(compile_ok);

    _prog.create(prog_name);
    _prog.setPersistent(true);
    _prog.attachShader(vshader);
    _prog.attachShader(fshader);
    link_ok = _prog.link();
    assert(link_ok);
  }



