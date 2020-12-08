#pragma once

#include <core/integrator.h>

namespace helios
{
class DirectLightingIntegrator : public Integrator
{
public:
    using Ptr = std::shared_ptr<DirectLightingIntegrator>;

public:
    DirectLightingIntegrator(vk::Backend::Ptr backend);
    ~DirectLightingIntegrator();

protected:
    void execute(RenderState& render_state) override;
    void gather_debug_rays(const glm::ivec2& pixel_coord, const uint32_t& num_debug_rays, const glm::mat4& view, const glm::mat4& projection, RenderState& render_state) override;
    void create_pipeline();
    void create_ray_debug_pipeline();

private:
    vk::DescriptorSet::Ptr      m_ds[2];
    vk::RayTracingPipeline::Ptr m_pipeline;
    vk::PipelineLayout::Ptr     m_pipeline_layout;
    vk::ShaderBindingTable::Ptr m_sbt;

    vk::RayTracingPipeline::Ptr m_ray_debug_pipeline;
    vk::PipelineLayout::Ptr     m_ray_debug_pipeline_layout;
    vk::ShaderBindingTable::Ptr m_ray_debug_sbt;
};
} // namespace helios