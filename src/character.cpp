#include "character.hpp"
#include "image.hpp"
#include "renderable_component.hpp"
#include <glog/logging.h>
#include <iostream>
#include <fstream>


#ifdef USE_GLES

#include <GLES2/gl2.h>

#endif

#ifdef USE_GL

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#endif

#define TILESET_ELEMENT_SIZE 16
#define GLOBAL_SCALE 2
#define IMAGE2_NUM_COMPONENTS 4




Character::Character() {
    init_shaders();
    load_textures();
    generate_tex_data();
    generate_vertex_data();
    LOG(INFO) << "Character initialized";
}

Character::~Character() {
    delete[] sprite_tex_data;
    delete[] sprite_data;
    delete texture_image;

    LOG(INFO) << "Character destructed";
}


void Character::generate_tex_data() {
    RenderableComponent* render_component = get_renderable_component();
    if(render_component == nullptr) {
        LOG(ERROR) << "ERROR in Character::generate_tex_data, render_component is nullptr";
        return;
    }
  
    //holds the texture data
    //need 12 float for the 2D texture coordinates
    int num_floats = 12;
    sprite_tex_data = new GLfloat[sizeof(GLfloat)*num_floats]; 
    if(sprite_tex_data == nullptr) {
        LOG(ERROR) << "ERROR in Character::generate_tex_data, cannot allocate memory";
        return;
    }

    GLfloat offset_x = GLfloat(TILESET_ELEMENT_SIZE) / (GLfloat)texture_image->store_width;
    GLfloat offset_y = GLfloat(TILESET_ELEMENT_SIZE) / (GLfloat)texture_image->store_height;

    //bottom left
    sprite_tex_data[0]  = offset_x * GLfloat(4.0);
    sprite_tex_data[1]  = offset_y * GLfloat(7.0);

    //top left
    sprite_tex_data[2]  = offset_x * GLfloat(4.0);
    sprite_tex_data[3]  = offset_y * GLfloat(8.0); 

    //bottom right
    sprite_tex_data[4]  = offset_x * GLfloat(5.0);
    sprite_tex_data[5]  = offset_y * GLfloat(7.0);

    //top left
    sprite_tex_data[6]  = offset_x * GLfloat(4.0);
    sprite_tex_data[7]  = offset_y * GLfloat(8.0);

    //top right
    sprite_tex_data[8]  = offset_x * GLfloat(5.0);
    sprite_tex_data[9]  = offset_y * GLfloat(8.0);

    //bottom right
    sprite_tex_data[10] = offset_x * GLfloat(5.0);
    sprite_tex_data[11] = offset_y * GLfloat(7.0);

    render_component->set_texture_coords_data(sprite_tex_data, sizeof(GLfloat)*num_floats, false);
} 

void Character::generate_vertex_data() {
    RenderableComponent* render_component = get_renderable_component();
    if(render_component == nullptr) {
        LOG(ERROR) << "Character::generate_vertex_data: render_component is nullptr";
        return;
    }

    //holds the character vertex data
    //need 18 floats for each coordinate as these hold 3D coordinates
    int num_floats = 18;
    sprite_data  = new GLfloat[sizeof(GLfloat)*num_floats]; 
    if(sprite_data == nullptr) {
        LOG(ERROR) << "Characater::generate_vertex_data: cannot allocate memory";
        return;
    }

    float scale = TILESET_ELEMENT_SIZE * GLOBAL_SCALE;
    float depth = 0.0f;
    //bottom left 
    sprite_data[0] = 0;
    sprite_data[1] = 0;
    sprite_data[2] = depth;

    //top left
    sprite_data[3] = 0;
    sprite_data[4] = scale;
    sprite_data[5] = depth;

    //bottom right
    sprite_data[6] = scale;
    sprite_data[7] = 0;
    sprite_data[8] = depth;

    //top left
    sprite_data[9] = 0;
    sprite_data[10] = scale;
    sprite_data[11] = depth;

    //top right
    sprite_data[12] = scale;
    sprite_data[13] = scale;
    sprite_data[14] = depth;

    //bottom right
    sprite_data[15] = scale;
    sprite_data[16] = 0;
    sprite_data[17] = depth;

    render_component->set_vertex_data(sprite_data,sizeof(GLfloat)*num_floats, false);
    render_component->set_num_vertices_render(num_floats/3);//GL_TRIANGLES being used
}

void Character::load_textures() {

    RenderableComponent* render_component = get_renderable_component();
    if(render_component == nullptr) {
        LOG(ERROR) << "Character::generate_tex_data: render_component is nullptr";
        return;
    }


    texture_image = new Image("../resources/characters_1.png");

    //Set the texture data in the rederable component
    render_component->set_texture_image(texture_image);
}

bool Character::init_shaders() {
    Shader* shader = nullptr;
    try {
#ifdef USE_GLES
        shader = new Shader("vert_shader.glesv", "frag_shader.glesf");
#endif
#ifdef USE_GL
        shader = new Shader("vert_shader.glv", "frag_shader.glf");
#endif
    }
    catch (std::exception e) {
        delete shader;
        shader = nullptr;
        LOG(ERROR) << "Failed to create the shader";
        return false;
    }

    RenderableComponent* render_component = get_renderable_component();
    if(render_component == nullptr) {
        LOG(ERROR) << "Character::init_shaders: render_component is nullptr";
        delete shader;
    }

    //Set the shader
    render_component->set_shader(shader);

    return true;

}
