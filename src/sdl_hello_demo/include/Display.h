#ifndef DISPLAY_H
#define DISPLAY_H

#include <memory>
#include <string>

class Display
{
public:
    explicit Display(int width, int height, const std::string_view &title);
    virtual ~Display();

public:
    void update();

    bool isClosed() const;

    void clear(float r, float g, float b, float a);

private:
    class PImpl;
    std::unique_ptr<PImpl> impl_;
};


#endif // DISPLAY_H
