#include <pch.h>
#include "Terrain/TerrainAnalysis.h"
#include "Terrain/MapMath.h"
#include "Agent/AStarAgent.h"
#include "Terrain/MapLayer.h"
#include "Projects/ProjectThree.h"

#include <iostream>

constexpr int GRID_X_MAX = 40;
constexpr int GRID_Y_MAX = 40;


bool ProjectThree::implemented_fog_of_war() const // extra credit
{
    return false;
}

float distance_to_closest_wall(int row, int col)
{
    /*
        Check the euclidean distance from the given cell to every other wall cell,
        with cells outside the map bounds treated as walls, and return the smallest
        distance.  Make use of the is_valid_grid_position and is_wall member
        functions in the global terrain to determine if a cell is within map bounds
        and a wall, respectively.
    */

    // WRITE YOUR CODE HERE
    
    float min_distance = std::numeric_limits<float>::max();

    
    for (int y = -1; y <= GRID_Y_MAX; ++y)
    {
        for (int x = -1; x <= GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
            {
                float distance = std::sqrtf(static_cast<float>((x - col) * (x - col) + (y - row) * (y - row)));

                if (distance < min_distance)
                {
                    min_distance = distance;
                }
            }
        }
    }

    return min_distance;
}

bool isCrossingGridBox(int row0, int col0, int row1, int col1, int row_grid, int col_grid)
{
    float y_grid = static_cast<float>(row_grid);
    float x_grid = static_cast<float>(col_grid);

    constexpr float GRID_THRESHOLD = 0.51f;

    /*
    0 1
    2 3
    */
    Vec2 grid_corners[4] = { Vec2(x_grid - GRID_THRESHOLD, y_grid + GRID_THRESHOLD), Vec2(x_grid + GRID_THRESHOLD, y_grid + GRID_THRESHOLD)
                            , Vec2(x_grid - GRID_THRESHOLD, y_grid - GRID_THRESHOLD) , Vec2(x_grid + GRID_THRESHOLD, y_grid - GRID_THRESHOLD) };

    float y0 = static_cast<float>(row0);
    float x0 = static_cast<float>(col0);

    float y1 = static_cast<float>(row1);
    float x1 = static_cast<float>(col1);

    if (col0 == col1)
    {
        /*
           |
         0 | 1
         2 | 3
           |
        */

        bool firstResult = grid_corners[0].x > x0;

        if (firstResult != grid_corners[1].x > x1) 
        {
            return true;
        }
    }

    Vec2 grid_pos = grid_corners[0];

    float first_result = (y1 - y0) * (grid_pos.x - x0) - (x1 - x0) * (grid_pos.y - y0);

    for (int i = 1; i < 4; ++i)
    {
        Vec2 grid_pos = grid_corners[i];
        float result = (y1 - y0) * (grid_pos.x - x0) - (x1 - x0) * (grid_pos.y - y0);
        
        if (std::signbit(first_result) != std::signbit(result))
        {
            return true;
        }
    }

    return false;
}

bool is_clear_path(int row0, int col0, int row1, int col1)
{
    /*
        Two cells (row0, col0) and (row1, col1) are visible to each other if a line
        between their centerpoints doesn't intersect the four boundary lines of every
        wall cell.  You should puff out the four boundary lines by a very tiny amount
        so that a diagonal line passing by the corner will intersect it.  Make use of the
        line_intersect helper function for the intersection test and the is_wall member
        function in the global terrain to determine if a cell is a wall or not.
    */

    // WRITE YOUR CODE HERE

    int row_min = row0 < row1 ? row0 : row1;
    int col_min = col0 < col1 ? col0 : col1;
    int row_max = row0 > row1 ? row0 : row1;
    int col_max = col0 > col1 ? col0 : col1;

    for (int y = row_min; y <= row_max; ++y)
    {
        for (int x = col_min; x <= col_max; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) ||
                (x == col0 && y == row0) ||
                (x == col1 && y == row1))
                continue;

            if (terrain->is_wall(y, x))
            {
                if (isCrossingGridBox(row0, col0, row1, col1, y, x))
                    return false;
            }
            
        }
    }


    return true; // REPLACE THIS
}

void analyze_openness(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the value 1 / (d * d),
        where d is the distance to the closest wall or edge.  Make use of the
        distance_to_closest_wall helper function.  Walls should not be marked.
    */

    // WRITE YOUR CODE HERE
    for (int y = 0; y < GRID_Y_MAX; ++y)
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;

            float distance = distance_to_closest_wall(y, x);
            layer.set_value(y, x, 1 / (distance * distance));
        }
    }
}

void analyze_visibility(MapLayer<float> &layer)
{
    /*
        Mark every cell in the given layer with the number of cells that
        are visible to it, divided by 160 (a magic number that looks good).  Make sure
        to cap the value at 1.0 as well.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
    constexpr float A_MAGIC_NUMBER_THAT_LOOKS_GOOD = 160.f;

    // I HATE THIS
    for (int y = 0; y < GRID_Y_MAX; ++y) 
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;

            int visibility = 0;

            for (int y_another = 0; y_another < GRID_Y_MAX; ++y_another)
            {
                for (int x_another = 0; x_another < GRID_X_MAX; ++x_another)
                {
                    if (!terrain->is_valid_grid_position(y_another, x_another) || 
                        terrain->is_wall(y_another, x_another) || 
                        (x == x_another && y == y_another))
                        continue;

                    if (is_clear_path(y, x, y_another, x_another)) // total n^6 loop wow
                        ++visibility;
                }
            }

            layer.set_value(y, x, static_cast<float>(visibility) / A_MAGIC_NUMBER_THAT_LOOKS_GOOD);
        }
    }
}

void analyze_visible_to_cell(MapLayer<float> &layer, int row, int col)
{
    /*
        For every cell in the given layer mark it with 1.0
        if it is visible to the given cell, 0.5 if it isn't visible but is next to a visible cell,
        or 0.0 otherwise.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
    for (int y = 0; y < GRID_Y_MAX; ++y)
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;
            
            if (is_clear_path(row, col, y, x))
            {
                layer.set_value(y, x, 1.f);
            }
        }
    }

    GridPos neighbor_val[8] = {
        (1,-1),(1,0),(1,1),
        (0,-1),        (0,1),
        (-1,-1),(-1,0),(-1,1)
    };

    for (int y = 0; y < GRID_Y_MAX; ++y)
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;
            if (layer.get_value(y, x) == 1.f)
                continue;

            for (int i = 0; i < 8; ++i)
            {
                int row_neighbor = y + neighbor_val[i].row;
                int col_neighbor = x + neighbor_val[i].col;
                if (terrain->is_valid_grid_position(row_neighbor, col_neighbor) && layer.get_value(row_neighbor, col_neighbor) == 1.f)
                {
                    layer.set_value(y, x, 0.5f);
                }
            }
        }
    }
}

void analyze_agent_vision(MapLayer<float> &layer, const Agent *agent)
{
    /*
        For every cell in the given layer that is visible to the given agent,
        mark it as 1.0, otherwise don't change the cell's current value.

        You must consider the direction the agent is facing.  All of the agent data is
        in three dimensions, but to simplify you should operate in two dimensions, the XZ plane.

        Take the dot product between the view vector and the vector from the agent to the cell,
        both normalized, and compare the cosines directly instead of taking the arccosine to
        avoid introducing floating-point inaccuracy (larger cosine means smaller angle).

        Give the agent a field of view slighter larger than 180 degrees.

        Two cells are visible to each other if a line between their centerpoints doesn't
        intersect the four boundary lines of every wall cell.  Make use of the is_clear_path
        helper function.
    */

    // WRITE YOUR CODE HERE
    constexpr float FOV = PI;
    constexpr float THRESHOLD = 0.0001f;
    const float COS_FOV = cosf(FOV/2 + THRESHOLD);

    for (int y = 0; y < GRID_Y_MAX; ++y)
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;

            const Vec3& grid_pos = terrain->get_world_position(y, x);
            const Vec3& agent_pos = agent->get_position();
            
            Vec2 grid_normal_vec2 = Vec2(grid_pos.x, grid_pos.z) - Vec2(agent_pos.x, agent_pos.z);
            grid_normal_vec2.Normalize();
            
            const Vec3& temp_forward_vec = agent->get_forward_vector();
            Vec2 agent_forward_vec2 = Vec2(temp_forward_vec.x, temp_forward_vec.z);
            agent_forward_vec2.Normalize();
            
            float dot_val = agent_forward_vec2.Dot(grid_normal_vec2);

            GridPos agent_grid_pos = terrain->get_grid_position(agent_pos);
            if (dot_val > COS_FOV &&
                is_clear_path(agent_grid_pos.row, agent_grid_pos.col, y, x))
            {
                layer.set_value(y, x, 1.f);
            }
        }
    }
}

float get_max_influence(const GridPos& grid, MapLayer<float>& layer, float decay)
{
    const float SQRT2 = std::sqrtf(2.f);

    float influence_max = 0.f;

    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (i == 0 && j == 0) // I'm too lazy to make array again
                continue;

            int row = grid.row + i;
            int col = grid.col + j;

            if (!terrain->is_valid_grid_position(row, col) || terrain->is_wall(row, col))
                continue;
            
            if (i != 0 && j != 0)
            {
                if (terrain->is_wall(row, grid.col) || terrain->is_wall(grid.row, col))
                {
                    continue;
                }
            }


            float distance = i != 0 && j != 0 ? SQRT2 : 1;

            float influence_old = layer.get_value(row, col);
            float influence_new = influence_old * std::expf(-distance * decay);

            if (influence_new > influence_max)
            {
                influence_max = influence_new;
            }
        }
    }

    return influence_max;
}

void propagate_solo_occupancy(MapLayer<float> &layer, float decay, float growth)
{
    /*
        For every cell in the given layer:

            1) Get the value of each neighbor and apply decay factor
            2) Keep the highest value from step 1
            3) Linearly interpolate from the cell's current value to the value from step 2
               with the growing factor as a coefficient.  Make use of the lerp helper function.
            4) Store the value from step 3 in a temporary layer.
               A float[40][40] will suffice, no need to dynamically allocate or make a new MapLayer.

        After every cell has been processed into the temporary layer, write the temporary layer into
        the given layer;
    */
    
    // WRITE YOUR CODE HERE
    float tempLayer[GRID_Y_MAX][GRID_X_MAX] = { 0.f };

    for (int y = 0; y < GRID_Y_MAX; ++y)
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;

            float influence_current = layer.get_value(y, x);
            float influence_max = get_max_influence(GridPos(y, x), layer, decay);

            float influence_new = std::lerp(influence_current, influence_max, growth);

            tempLayer[y][x] = influence_new;
        }
    }

    for (int y = 0; y < GRID_Y_MAX; ++y)
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;

            layer.set_value(y, x, tempLayer[y][x]);
        }
    }
}

// NEED TO FIX THIS FUNCTION
void normalize_solo_occupancy(MapLayer<float> &layer)
{
    /*
        Determine the maximum value in the given layer, and then divide the value
        for every cell in the layer by that amount.  This will keep the values in the
        range of [0, 1].  Negative values should be left unmodified.
    */

    // WRITE YOUR CODE HERE

    float influence_max = 0.f;

    for (int y = 0; y < GRID_Y_MAX; ++y)
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;

            float influence = layer.get_value(y, x);

            if (influence > influence_max)
            {
                influence_max = influence;
            }
        }
    }

    layer.for_each([influence_max](float& val)
        {
            if (val > 0)
            {
                val /= influence_max;
            }
        });
}

void enemy_field_of_view(MapLayer<float> &layer, float fovAngle, float closeDistance, float occupancyValue, AStarAgent *enemy)
{
    /*
        First, clear out the old values in the map layer by setting any negative value to 0.
        Then, for every cell in the layer that is within the field of view cone, from the
        enemy agent, mark it with the occupancy value.  Take the dot product between the view
        vector and the vector from the agent to the cell, both normalized, and compare the
        cosines directly instead of taking the arccosine to avoid introducing floating-point
        inaccuracy (larger cosine means smaller angle).

        If the tile is close enough to the enemy (less than closeDistance),
        you only check if it's visible to enemy.  Make use of the is_clear_path
        helper function.  Otherwise, you must consider the direction the enemy is facing too.
        This creates a radius around the enemy that the player can be detected within, as well
        as a fov cone.
    */

    // WRITE YOUR CODE HERE

    layer.for_each([](float& val)
        {
            if (val < 0.f)
            {
                val = 0.f;
            }
        });

    const float fov_angle_radian = fovAngle / 180.f * PI; // It was degree...

    constexpr float THRESHOLD = 0.0001f;
    const float COS_FOV = cosf(fov_angle_radian / 2 + THRESHOLD);

    for (int y = 0; y < GRID_Y_MAX; ++y)
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;

            Vec3 grid_pos = terrain->get_world_position(y, x);
            Vec3 enemy_pos = enemy->get_position();
            GridPos enemy_grid_pos = terrain->get_grid_position(enemy_pos);

            int distant_sq = (enemy_grid_pos.col - x) * (enemy_grid_pos.col - x) + (enemy_grid_pos.row - y) * (enemy_grid_pos.row - y);
            
            if (static_cast<float>(distant_sq) < closeDistance * closeDistance)
            {
                
                if (is_clear_path(enemy_grid_pos.row, enemy_grid_pos.col, y, x))
                {
                    layer.set_value(y, x, occupancyValue);
                    continue;
                }
            }

            Vec2 grid_normal_vec2 = Vec2(grid_pos.x, grid_pos.z) - Vec2(enemy_pos.x, enemy_pos.z);
            grid_normal_vec2.Normalize();

            Vec3 temp_forward_vec = enemy->get_forward_vector();
            Vec2 enemy_forward_vec2 = Vec2(temp_forward_vec.x, temp_forward_vec.z);
            enemy_forward_vec2.Normalize();

            float dot_val = enemy_forward_vec2.Dot(grid_normal_vec2);

            if (dot_val > COS_FOV)
            {
                if (is_clear_path(enemy_grid_pos.row, enemy_grid_pos.col, y, x))
                {
                    layer.set_value(y, x, occupancyValue);
                }
            }
        }
    }
}

bool enemy_find_player(MapLayer<float> &layer, AStarAgent *enemy, Agent *player)
{
    /*
        Check if the player's current tile has a negative value, ie in the fov cone
        or within a detection radius.
    */

    const auto &playerWorldPos = player->get_position();

    const auto playerGridPos = terrain->get_grid_position(playerWorldPos);

    // verify a valid position was returned
    if (terrain->is_valid_grid_position(playerGridPos) == true)
    {
        if (layer.get_value(playerGridPos) < 0.0f)
        {
            return true;
        }
    }

    // player isn't in the detection radius or fov cone, OR somehow off the map
    return false;
}

bool enemy_seek_player(MapLayer<float> &layer, AStarAgent *enemy)
{
    /*
        Attempt to find a cell with the highest nonzero value (normalization may
        not produce exactly 1.0 due to floating point error), and then set it as
        the new target, using enemy->path_to.

        If there are multiple cells with the same highest value, then pick the
        cell closest to the enemy.

        Return whether a target cell was found.
    */

    // WRITE YOUR CODE HERE
    GridPos target_pos(-1,-1);
    float influence_max = 0.f;

    for (int y = 0; y < GRID_Y_MAX; ++y)
    {
        for (int x = 0; x < GRID_X_MAX; ++x)
        {
            if (!terrain->is_valid_grid_position(y, x) || terrain->is_wall(y, x))
                continue;

            float influence = layer.get_value(y, x);

            if (influence > influence_max)
            {
                target_pos = GridPos(y, x);
                influence_max = influence;
            }
        }
    }

    if (target_pos == GridPos(-1,-1))
    {
        return false;
    }
    else
    {
        enemy->path_to(terrain->get_world_position(target_pos));
        return true;
    }

    
}
