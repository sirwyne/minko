/*
Copyright (c) 2014 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "minko/Minko.hpp"
#include "minko/MinkoPNG.hpp"
#include "minko/MinkoSDL.hpp"
#include "minko/MinkoHTTP.hpp"

#if !defined(EMSCRIPTEN) // FIXME: Automate this in the HTTPLoader
# include "minko/net/HTTPWorker.hpp"
#endif

using namespace minko;
using namespace minko::component;
using namespace minko::math;

const std::string TEXTURE_FILENAME = "http://static.aerys.in:8080/minko3/http/box.png";

int
main(int argc, char** argv)
{
    auto canvas = Canvas::create("Minko Example - HTTP");

#if !defined(EMSCRIPTEN) // FIXME: Automate this in the HTTPLoader
    canvas->registerWorker<net::HTTPWorker>("http");
#endif

    auto sceneManager = SceneManager::create(canvas);
    auto defaultOptions = sceneManager->assets()->loader()->options();

    defaultOptions
        ->resizeSmoothly(true)
        ->generateMipmaps(true)
        ->loadAsynchronously(true);

    sceneManager->assets()->loader()->queue("effect/Basic.effect")->load();

    // setup assets
    defaultOptions->loadAsynchronously(true)
        ->registerProtocol<net::HTTPProtocol>("http")
        ->registerProtocol<net::HTTPProtocol>("https")
        ->registerParser<file::PNGParser>("png");

    sceneManager->assets()->loader()->queue(TEXTURE_FILENAME);

    sceneManager->assets()->geometry("cube", geometry::CubeGeometry::create(sceneManager->assets()->context()));

    auto root = scene::Node::create("root")
        ->addComponent(sceneManager);

    auto camera = scene::Node::create("camera")
        ->addComponent(Renderer::create(0x7f7f7fff))
        ->addComponent(Transform::create(
            Matrix4x4::create()->lookAt(Vector3::zero(), Vector3::create(0.f, 0.f, 3.f))
        ))
        ->addComponent(PerspectiveCamera::create(canvas->aspectRatio()));

    root->addChild(camera);

    auto mesh = scene::Node::create("mesh")
        ->addComponent(Transform::create());

    root->addChild(mesh);

    auto resized = canvas->resized()->connect([&](AbstractCanvas::Ptr canvas, uint w, uint h)
    {
        camera->component<PerspectiveCamera>()->aspectRatio(float(w) / float(h));
    });

    auto enterFrame = canvas->enterFrame()->connect([&](Canvas::Ptr canvas, float time, float deltaTime)
    {
        mesh->component<Transform>()->matrix()->appendRotationY(.01f);

        sceneManager->nextFrame(time, deltaTime);
    });

    auto _ = sceneManager->assets()->loader()->complete()->connect([=](file::Loader::Ptr loader)
    {
        std::cout << "main(): asset complete" << std::endl;

        mesh->addComponent(Surface::create(
            sceneManager->assets()->geometry("cube"),
            material::BasicMaterial::create()->diffuseMap(sceneManager->assets()->texture(TEXTURE_FILENAME)),
            sceneManager->assets()->effect("effect/Basic.effect")
        ));
    });

    sceneManager->assets()->loader()->load();
    canvas->run();
}


