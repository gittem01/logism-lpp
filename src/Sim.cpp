#include <ThinDrawer.h>
#include <CircuitUnit.h>
#include <Connector.h>
#include <Line.h>
#include <Camera.h>

Sim::Sim()
{
    memset(this->grid, 0, sizeof(this->grid));
    memset(this->dotGrid, 0, sizeof(this->dotGrid));

    this->td = new ThinDrawer();

    units.push_back(new CircuitUnit(this));

    for (CircuitUnit* c : units)
    {
        c->prepareVertexData();
        c->prepareUniforms();
        c->setupDescriptorSetLayout();
        c->preparePipeline();
        c->setupDescriptorSet();
    }

    for (int frameNum = 0; frameNum < td->frames.size(); frameNum++)
    {
        this->td->b_buildCommandBuffers(frameNum);
        for (CircuitUnit* c : units)
        {
            c->prepareRender(frameNum);
        }
        this->td->e_buildCommandBuffers(frameNum);
    }
}

void Sim::lupp()
{
    while (!glfwWindowShouldClose(td->wh->window))
    {
        ioH();

        td->p_renderLoop();

        for (CircuitUnit* c : units)
        {
            c->fillUniforms(td->lastSwapChainImageIndex);
        }

        td->s_renderLoop();

        if (td->needsSurfaceRecreation)
        {
            surfaceRecreation();
            continue;
        }
    }

    glfwTerminate();
}

void Sim::surfaceRecreation()
{
    vkDeviceWaitIdle(td->logicalDevice);

    for (CircuitUnit* c : units)
    {
        vkDestroyPipeline(td->logicalDevice, c->body->pipeline, VK_NULL_HANDLE);
    }

    td->surfaceRecreate();

    for (CircuitUnit* c : units)
    {
        c->preparePipeline();
    }

    for (int frameNum = 0; frameNum < td->frames.size(); frameNum++)
    {
        this->td->b_buildCommandBuffers(frameNum);
        for (CircuitUnit* c : units)
        {
            c->prepareRender(frameNum);
        }
        this->td->e_buildCommandBuffers(frameNum);
    }

    vkDeviceWaitIdle(td->logicalDevice);

    td->needsSurfaceRecreation = false;
}

void Sim::ioH()
{
    glm::vec2 mousePos = td->wh->cam->getMouseCoords();
    if (mousePos.x < Sim::midPosX && mousePos.x > -midPosX &&
        mousePos.y < Sim::midPosY && mousePos.y > -midPosY)
    {
        int xPos = (int)std::round(mousePos.x + Sim::midPosX);
        int yPos = (int)std::round(mousePos.y + Sim::midPosY);
        CircuitUnit* ptr = dotGrid[yPos][xPos];
        if (ptr)
        {
            if (td->wh->mouseData[2] > 0.0f && !startPos)
            {
                startPos = (grid_vector*)malloc(sizeof(grid_vector));
                *startPos = {xPos - Sim::midPosX, yPos - Sim::midPosY};

                Line* line1 = new Line(this);
                line1->position = *startPos;
                line1->size = *startPos;

                Line* line2 = new Line(this);
                line2->position = *startPos;
                line2->size = *startPos;

                units.push_back(line1);
                units.push_back(line2);

                for (int i = 0; i < 2; i++)
                {
                    CircuitUnit* c = units.at(units.size() - 1 - i);
                    c->prepareVertexData();
                    c->prepareUniforms();
                    c->setupDescriptorSetLayout();
                    c->preparePipeline();
                    c->setupDescriptorSet();
                }

                td->needsSurfaceRecreation = true;
            }

            if (ptr->unitType == DOT)
            {
                ((Connector*)ptr)->sizeMult = 1.5f;
            }
        }

        if (startPos)
        {
            if (td->wh->mouseData[2] == 0.0f)
            {
                free(startPos);
                startPos = NULL;

                Line* line1 = (Line*)units.at(units.size() - 2);
                Line* line2 = (Line*)units.at(units.size() - 1);

                if (isHoriz)
                {
                    grid_vector pos = line1->position;
                    int mult = line1->size.x > line1->position.x ? 1 : -1;
                    for (int i = 0; i < abs(line1->size.x - line1->position.x); i++)
                    {
                        pos.x += 1 * mult;
                        dotGrid[pos.y + Sim::midPosY][pos.x + Sim::midPosX] = line1;
                    }

                    pos = line2->position;
                    mult = line2->size.y > line2->position.y ? 1 : -1;
                    for (int i = 0; i < abs(line2->size.y - line2->position.y); i++)
                    {
                        pos.y += 1 * mult;
                        dotGrid[pos.y + Sim::midPosY][pos.x + Sim::midPosX] = line2;
                    }
                }
                else
                {
                    grid_vector pos = line1->position;
                    int mult = line1->size.y > line1->position.y ? 1 : -1;
                    for (int i = 0; i < abs(line1->size.y - line1->position.y); i++)
                    {
                        pos.y += 1 * mult;
                        dotGrid[pos.y + Sim::midPosY][pos.x + Sim::midPosX] = line1;
                    }

                    pos = line2->position;
                    mult = line2->size.x > line2->position.x ? 1 : -1;
                    for (int i = 0; i < abs(line2->size.x - line2->position.x); i++)
                    {
                        pos.x += 1 * mult;
                        dotGrid[pos.y + Sim::midPosY][pos.x + Sim::midPosX] = line2;
                    }
                }
            }
            else
            {
                Line* line1 = (Line*)units.at(units.size() - 2);
                Line* line2 = (Line*)units.at(units.size() - 1);

                if (isHoriz)
                {
                    if (line1->position.x == line1->size.x)
                    {
                        isHoriz = 0;
                    }
                }
                else
                {
                    if (line1->position.y == line1->size.y)
                    {
                        isHoriz = 1;
                    }
                }

                if (isHoriz)
                {
                    line1->size.y = startPos->y;
                    line1->size.x = (int)xPos - Sim::midPosX;
                    line2->position.x = line1->size.x;
                    line2->size.x = line2->position.x;
                    line2->size.y = (int)yPos - Sim::midPosY;
                }
                else
                {
                    line1->size.x = startPos->x;
                    line1->size.y = (int)yPos - Sim::midPosY;
                    line2->position.y = line1->size.y;
                    line2->size.y = line2->position.y;
                    line2->size.x = (int)xPos - Sim::midPosX;
                }

            }
        }
    }
}

void Sim::fillGrid(CircuitUnit* ptr, grid_vector position, grid_vector size)
{
    for (int x = 0; x < size.x; x++)
    {
        for (int y = 0; y < size.x; y++)
        {
            grid[Sim::midPosY + position.y + y][Sim::midPosX + position.x + x] = ptr;
        }
    }
}