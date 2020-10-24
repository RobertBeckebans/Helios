#pragma once

#include <vk.h>
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <memory>

namespace lumen
{
class Mesh;
class Material;
class TextureCube;

class Scene
{
public:
    using Ptr = std::shared_ptr<Scene>;

    friend class ResourceManager;

    enum NodeType
    {
        NODE_MESH,
        NODE_CAMERA,
        NODE_DIRECTIONAL_LIGHT,
        NODE_SPOT_LIGHT,
        NODE_POINT_LIGHT,
        NODE_IBL
    };

    class Node
    {
    public:
        using Ptr = std::shared_ptr<Node>;

    protected:
        NodeType                           m_type;
        bool                               m_enabled = true;
        std::string                        m_name;
        Node*                              m_parent = nullptr;
        std::vector<std::shared_ptr<Node>> m_children;

    public:
        Node(const NodeType& type, const std::string& name);
        ~Node();
        
        virtual void update() = 0;
        void         add_child(Node::Ptr child);
        Node::Ptr    find_child(const std::string& name);
        void         remove_child(const std::string& name);
    };

    class TransformNode : public Node
    {
    public:
        using Ptr = std::shared_ptr<TransformNode>;

    protected:
        bool      m_is_dirty                   = true;
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::quat m_orientation = glm::quat(glm::radians(glm::vec3(0.0f)));
        glm::vec3 m_scale = glm::vec3(1.0f);
        glm::mat4 m_prev_model_matrix = glm::mat4(1.0f);
        glm::mat4 m_model_matrix = glm::mat4(1.0f);
        glm::mat4 m_model_matrix_without_scale = glm::mat4(1.0f);

    public:
        TransformNode(const NodeType& type, const std::string& name);
        ~TransformNode();

        void update() override;

        glm::vec3 forward();
        glm::vec3 up();
        glm::vec3 left();
        glm::vec3 position();
        void      set_orientation_from_euler_yxz(const glm::vec3& e);
        void      set_orientation_from_euler_xyz(const glm::vec3& e);
        void      set_position(const glm::vec3& position);
        void      rotate_euler_yxz(const glm::vec3& e);
        void      rotate_euler_xyz(const glm::vec3& e);
    };

    class MeshNode : public TransformNode
    {
    public:
        using Ptr = std::shared_ptr<MeshNode>;

    private:
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<Material> m_material_override;

    public:
        MeshNode(const std::string& name);
        ~MeshNode();

        void update() override;

        inline void set_mesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }
        inline void set_material_override(std::shared_ptr<Material> material_override) { m_material_override = material_override; }
        inline std::shared_ptr<Mesh> mesh() { return m_mesh; }
        inline std::shared_ptr<Material> material_override() { return m_material_override; }
    };

    class DirectionalLightNode : public TransformNode
    {
    public:
        using Ptr = std::shared_ptr<TransformNode>;

    private:
        glm::vec3 m_color;
        float     m_intensity;

    public:
        DirectionalLightNode(const std::string& name);
        ~DirectionalLightNode();

        void update() override;

        inline void      set_color(const glm::vec3& color) { m_color = color; }
        inline void      set_intensity(const float& intensity) { m_intensity = intensity; }
        inline glm::vec3 color() { return m_color; }
        inline float     intensity() { return m_intensity; }
    };

    class SpotLightNode : public TransformNode
    {
    public:
        using Ptr = std::shared_ptr<SpotLightNode>;

        glm::vec3 m_color;
        float     m_cone_angle;
        float     m_range;
        float     m_intensity;

    public:
        SpotLightNode(const std::string& name);
        ~SpotLightNode();

        void update() override;

        inline void      set_color(const glm::vec3& color) { m_color = color; }
        inline void      set_intensity(const float& intensity) { m_intensity = intensity; }
        inline void      set_range(const float& range) { m_range = range; }
        inline void      set_cone_angle(const float& cone_angle) { m_cone_angle = cone_angle; }
        inline glm::vec3 color() { return m_color; }
        inline float     intensity() { return m_intensity; }
        inline float     range() { return m_range; }
        inline float     cone_angle() { return m_cone_angle; }
    };

    class PointLightNode : public TransformNode
    {
    public:
        using Ptr = std::shared_ptr<PointLightNode>;

    private:
        glm::vec3 m_color;
        float     m_range;
        float     m_intensity;

    public:
        PointLightNode(const std::string& name);
        ~PointLightNode();

        void update() override;

        inline void      set_color(const glm::vec3& color) { m_color = color; }
        inline void      set_intensity(const float& intensity) { m_intensity = intensity; }
        inline void      set_range(const float& range) { m_range = range; }
        inline glm::vec3 color() { return m_color; }
        inline float     intensity() { return m_intensity; }
        inline float     range() { return m_range; }
    };

    class CameraNode : public TransformNode
    {
    public:
        using Ptr = std::shared_ptr<CameraNode>;

    private:
        float m_near_plane = 1.0f;
        float m_far_plane = 1000.0f;
        float m_fov = 60.0f;
        glm::mat4 m_view_matrix = glm::mat4(1.0f);
        glm::mat4 m_projection_matrix = glm::mat4(1.0f);

    public:
        CameraNode(const std::string& name);
        ~CameraNode();

        void update() override;

        inline void  set_near_plane(const float& near_plane) { m_near_plane = near_plane; }
        inline void  set_far_plane(const float& far_plane) { m_far_plane = far_plane; }
        inline void  set_fov(const float& fov) { m_fov = fov; }
        inline float near_plane() { return m_near_plane; }
        inline float far_plane() { return m_far_plane; }
        inline float fov() { return m_fov; }
        inline glm::mat4 view_matrix() { return m_view_matrix; }
        inline glm::mat4 projection_matrix() { return m_projection_matrix; }
    };

    struct IBLNode : public Node
    {
    public:
        using Ptr = std::shared_ptr<IBLNode>;

    private:
        std::shared_ptr<TextureCube> m_image;
    
    public:
        IBLNode(const std::string& name);
        ~IBLNode();

        void update() override;

        inline void                         set_image(std::shared_ptr<TextureCube> image) { m_image = image; }
        inline std::shared_ptr<TextureCube> image() { return m_image; }
    };

public:
    Scene(vk::Backend::Ptr backend, Node::Ptr root);
    ~Scene();

    void update();

private:
    vk::AccelerationStructure::Ptr m_tlas;
    vk::DescriptorSet::Ptr         m_ds;
    Node::Ptr                      m_root;
};
} // namespace lumen