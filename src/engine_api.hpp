///
/// Defines the API used for the C++/Python integration
///

#ifndef ENGINE_API_H
#define ENGINE_API_H

#include <array>
#include <future>
#include <memory>
#include <utility>
#include <vector>

#include "gil_safe_future.hpp"
#include "notification.hpp"
#include "typeface.hpp"
#include "text_font.hpp"
#include "text.hpp"

#include "api.hpp"

class MapViewer;

enum Status {RUNNING, STOPPED, FAILED};
enum Direction {Next, Previous};

///
/// default python editor, used as long as another isn't passed as command line arg
#define DEFAULT_PY_EDITOR "gedit"

// Class wrapping the API calls into a static public class
class Engine {
private:

    ///
    /// name of editor used for python editing
    ///
    static std::string editor;

    static MapViewer* map_viewer;

    ///
    /// pointer for text box
    ///
    static Text* dialogue_box;

    ///
    /// cache for hold past notification
    ///
    static Notification notifcation_stack; 
    
    ///
    /// The size of a tile
    ///
    static int tile_size;

    ///
    /// The global tile and object scaling factor, how much to multiply widths and heights
    /// by
    ///
    static int global_scale;
public:
    ///
    /// Get the global scale 
    /// @return the global scale
    ///
    static int get_global_scale() { return global_scale; }
    
    ///
    /// Set the global scaling factor
    /// @param _global_scale the scaling factor
    ///
    static void set_global_scale(int _global_scale) { global_scale = _global_scale; }

    ///
    /// Set the tile size to be used by the engine
    /// @param _tile_size the tile size
    ///
    static void set_tile_size(int _tile_size) { tile_size = _tile_size; }

    ///
    /// Get the tile size used by the engine  in pixels - we only support square ones
    /// @return the tile size
    ///
    static int get_tile_size() { return tile_size; }

    static int get_actual_tile_size() {return tile_size * global_scale; }
    ///
    /// Set the map viewer attached to the engine
    /// @param _map_viewer the map viewer which is attached to the engine
    ///
    static void set_map_viewer(MapViewer* _map_viewer) { map_viewer = _map_viewer; }


    ///
    /// Get the map viewer attached to the engine
    /// @return a pointer to the map viewer
    ///
    static MapViewer* get_map_viewer() { return map_viewer;    }

    ///
    /// Move sprite onscreen
    ///
    /// @param id ID of sprite to move
    /// @param dx move in x by dx tiles
    /// @param dy move in x by dy tiles
    ///
    static void move_sprite(int id, Vec2D move_by);
    static void move_sprite(int id, Vec2D move_by, GilSafeFuture<bool> walk_succeeded_return);

    ///
    /// Determine if a location can be walked on
    /// @param x_pos the x position to test
    /// @param y_pos the y position to test
    /// @return true if the position can be walked on, false if not
    ///
    static bool walkable(Vec2D location);

    ///
    /// Change the tile in the map in the given layer at the provided position
    /// @param new_id the id of the new tile
    /// @param x the x position of the tile to change
    /// @param y the y position of the tile to change
    /// @param layer the layer of the tile to change
    /// @return indicates if the operation completed successfully
    ///
    static bool change_tile(Vec2D tile, int layer_num, int tile_id);

    ///
    /// Gets the ids of the tiles at this location. Layer 0 is the first
    /// element in the vector. Then each layer follows. If the id is 0
    /// then this  means that there is no tile in this layer
    /// @param x the x location to fetch the tile list for
    /// @param y the y location to fetch the tile list for
    /// @return a vector of the tiles, stored in layer order with layer 0
    /// as the first element. A 0 is stored for a layer if there is no
    /// tile there
    ///
    static std::vector<int> get_tiles(Vec2D location);

    ///
    /// Get the objects at the given map position if any, empty if no objects
    /// @param x the x position on the map
    /// @param y the y position on the map
    /// @return a vector of all the objects at that position on the map
    ///
    static std::vector<int> get_objects(Vec2D location);

    ///
    /// Get the sprites at the given map position if any, empty if no objects
    /// @param x the x position on the map
    /// @param y the y position on the map
    /// @return a vector of all the objects at that position on the map
    ///
    static std::vector<int> get_sprites(Vec2D location);

    ///
    /// Load the map specified by the ap id
    /// @param map_id the id of the map to load
    /// @return indicate if the map was successfully loaded
    ///
    static bool load_map(int map_id);

    ///
    /// Get the location of the map object or sprite in the map, throws exception if
    /// there is the object is not on the map
    /// @id the id of the object
    /// @return a pair which is the (x, y) tuple of the object position
    ///
    static Vec2D find_object(int id);

    ///
    /// Open a text editor for the user to edit a file
    /// @param filename name of file in scripts directory
    ///
    static bool open_editor(std::string filename);

    ///
    /// Get a list of objects at this point
    /// @return a vector of object ids
    ///
    static std::vector<int> get_objects_at(Vec2D location);

    ///
    /// Get a list of sprites at this point
    /// @return a vector of object ids
    ///
    static std::vector<int> get_sprites_at(Vec2D location);


    ///
    /// Add an object to the map at the given location
    /// @param object_id the id of the object to add
    /// @param x_pos the x position of the object
    /// @param y_pos the y position of the object
    ///
    static void add_object(int object_id, Vec2D location);

    ///
    /// Remove an object from the map at a givene location
    /// @param object_id the id of the object to add
    /// @param x_pos the x position of the object
    /// @param y_pos the y position of the object
    ///
    static void remove_object(int object_id, Vec2D location);

    ///
    /// set the text editor, opened by the challenges
    ///
    static void set_editor(std::string _editor) {editor = _editor; }

    static void set_dialogue_box(Text* _dialogue_box) {dialogue_box = _dialogue_box; }
    static Text* get_dialogue_box(){return dialogue_box; }

    static void print_dialogue(std::string name, std::string text);
    static void move_notification(Direction direction);

    static void text_displayer();
    static void text_updater();
    static void update_status(int id, std::string status);

};
#endif
