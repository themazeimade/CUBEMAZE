#include "renderobjects.h"
#include "imgui.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <stdint.h>

objProperties::objProperties() {
  // std::cout << "properties constructor called" << std::endl;
  fmass = 1.0f;

  vpos = glm::vec3(0.0f, 0.0f, 0.0f);
  vprevPos = glm::vec3(0.0f, 0.0f, 0.0f);
  fAngle = glm::radians(0.0f);

  vvelocity = glm::vec3(0.0f, 0.0f, 0.0f);
  fspeed = 0.0f;
  vforces = glm::vec3(0.0f, 0.0f, 0.0f);
  fNormalForce = 0.0f;
  fRadius = 0.3f;
  vgravity.x = 0.0f;
  vgravity.y = fmass * _GRAVITY;
  vImpactforces = glm::vec3(0.0f, 0.0f, 0.0f);
  vTangent = glm::vec3(0.0f);
  bObjectCollision = false;
  bCollision = false;
  goingOut = false;
};

void objProperties::CalcF() {
  // glm::vec3 vforcesImpostor = vforces;
  vforces = glm::vec3(0.f);
  

  if (bCollision == false) {
    
    vforces += vgravity;

    glm::vec3 vDrag(0.0f, 0.0f, 0.0f);
    float fDrag(0.0f);

    vDrag -= vvelocity;
    glm::normalize(vDrag);

    fDrag = static_cast<float>(
        0.5 * _AIRDENSITY * static_cast<double>(fspeed * fspeed) *
        static_cast<double>(3.14159f * fRadius * fRadius) * _DRAG);
    vDrag *= fDrag;
    vforces += vDrag;
    // std::cout << "hi" << std::endl;

    // // Wind
    // glm::vec3 vWind(0.0f, 0.0f, 0.0f);
    // vWind.x = static_cast<float>(
    //     0.5 * _AIRDENSITY * _WINDSPEED * _WINDSPEED *
    //     static_cast<double>(3.14159f * fRadius * fRadius) * _DRAG);
    // vforces += vWind;
  } else {
    vvelocity = {0.0f,-0.009f,0.0f};
    vforces += vImpactforces;
  vImpactforces = {0.0f, 0.0f, 0.0f};
  }
  // }
  //
  // // std::cout << "forces x: " << vforces.x << std::endl;
  // // std::cout << "forces y: " << vforces.y << std::endl;
  // bObjectCollision = false;
}

void objProperties::updateEuler(double dt) {
  glm::vec3 a;
  glm::vec3 dv;
  glm::vec3 ds(0.0f);

  a = vforces / fmass;
  // std::cout << "vforces x: " << vforces.x << "y: " << vforces.y << std::endl;
  dv = a * static_cast<float>(dt);
  vvelocity += dv;
  // std::cout << "vvelocity x: " << vvelocity.x << "y: " << vvelocity.y <<
  // std::endl;
  fspeed = glm::length(vvelocity);
  // if (fspeed < 0.06f) vvelocity = {0.0f,0.0f,0.0f};
  ds = vvelocity * static_cast<float>(dt);
  // if (bCollision == false)
  // vprevPos = vpos;

  // if (fspeed >= 0.1) {
  vpos += ds;

  // };
  // std::cout << "vprevPos = ("<< vprevPos.x << ";" << vprevPos.y << ")" <<
  // std::endl; std::cout << "vpos = ("<< vpos.x << ";" << vpos.y << ")" <<
  // std::endl;
}

void renderobject::createMeshBuffers() {
  if (mesh->vertices.empty() || mesh->indices.empty()) {
    throw std::runtime_error("no data to be attached to buffer");
  }
  context->createVertexBuffer(vertexBuffer, vertexBufferMemory, mesh->vertices);
  context->createIndexBuffer(indexBuffer, indexBufferMemory, mesh->indices);
  // context->mainDeletion.push_function([=]() { destroyMeshBuffers(); });
}

void renderobject::destroyMeshBuffers() {
  vkDestroyBuffer(context->getDevice(), indexBuffer, nullptr);
  vkFreeMemory(context->getDevice(), indexBufferMemory, nullptr);

  vkDestroyBuffer(context->getDevice(), vertexBuffer, nullptr);
  vkFreeMemory(context->getDevice(), vertexBufferMemory, nullptr);
}

void renderobject::createMVPBuffer() {
  VkDeviceSize bufferSize = sizeof(UniformBufferObject);

  uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
  uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
  uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    context->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          uniformBuffers[i], uniformBuffersMemory[i]);

    vkMapMemory(context->getDevice(), uniformBuffersMemory[i], 0, bufferSize, 0,
                &uniformBuffersMapped[i]);
  }
  // context->mainDeletion.push_function([=]() {
  //   for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
  //     vkDestroyBuffer(context->getDevice(), uniformBuffers[i], nullptr);
  //     vkFreeMemory(context->getDevice(), uniformBuffersMemory[i], nullptr);
  //   }
  // });
}

void renderobject::buildDescriptorSets() {

  std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT,
                                             context->getDLayout());
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = descriptorPool;
  allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
  allocInfo.pSetLayouts = layouts.data();

  descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
  if (vkAllocateDescriptorSets(context->getDevice(), &allocInfo,
                               descriptorSets.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate descriptor sets!");
  }

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = context->getTextureImageView();
    imageInfo.sampler = context->getTextureSampler();

    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptorSets[i];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = descriptorSets[i];
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(context->getDevice(),
                           static_cast<uint32_t>(descriptorWrites.size()),
                           descriptorWrites.data(), 0, nullptr);
  }
}

void renderobject::createDescriptorPool() {
  std::array<VkDescriptorPoolSize, 2> poolSizes{};
  poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

  if (vkCreateDescriptorPool(context->getDevice(), &poolInfo, nullptr,
                             &descriptorPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

void renderobject::prepareRenderProperties() {
  createMeshBuffers();
  createMVPBuffer();
  createDescriptorPool();
  buildDescriptorSets();
  createMeshPipeline();
}
void renderobject::injectMethods2commandB(VkCommandBuffer commandbuffer_,
                                          camera *_sceneCamera) {
  updateUBO(_sceneCamera);
  if (mesh->shaderPrimitive != true) {
    vkCmdBindPipeline(commandbuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      pipeline);
  } else {
    vkCmdBindPipeline(commandbuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      *(context->getdefaultPipeline()));
  }
  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandbuffer_, 0, 1, vertexBuffers, offsets);
  vkCmdBindIndexBuffer(commandbuffer_, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
  vkCmdBindDescriptorSets(commandbuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          PipeLayout, 0, 1,
                          &descriptorSets[context->getCurrFrame()], 0, nullptr);
  vkCmdDrawIndexed(commandbuffer_, static_cast<uint32_t>(mesh->indices.size()),
                   1, 0, 0, 0);
}
void renderobject::updateUBO(camera *_sceneCamera) {
  UniformBufferObject ubo_;
  // do stuff with that
  ubo_.model = glm::translate(ubo_.model, mesh->properties->vpos);
  ubo_.model = glm::rotate(ubo_.model,mesh->properties->fAngle, glm::vec3(1.0f,0.0f,0.0f));
  ubo_.view =
      glm::lookAt(_sceneCamera->getPosition(),
                  _sceneCamera->getPosition() + _sceneCamera->frontCamera,
                  _sceneCamera->upVector);
  ubo_.proj = _sceneCamera->getCameraProjM();

  memcpy(uniformBuffersMapped[context->getCurrFrame()], &ubo_, sizeof(ubo_));
}

void renderobject::createMeshPipeline() {
  if (mesh->shaderPrimitive != true) {
    context->createDerivativePipeline("../shaders/vert01.spv",
                                      "../shaders/frag01.spv", pipeline);
  }
}

void renderObjectQueue::push_renderobject(std::unique_ptr<renderobject> ro_) {
  if (ro_->mesh->transparent == true) {
    shapes.push_front(std::move(ro_));
    // std::cout << "pushed to the front" << std::endl;
    frontier++;
  } else {
    shapes.push_back(std::move(ro_));
    // std::cout << "pushed to the back" << std::endl;
  }
};

void renderObjectQueue::flush(VkCommandBuffer commandBuffer_) {
  for (auto it = shapes.rbegin(); it != shapes.rend(); it++) {
    (*it)->injectMethods2commandB(commandBuffer_, sceneCamera.get());
  }
}

void renderObjectQueue::flushGuiCalls() {
  int j = 0;
  for (auto it = shapes.rbegin() /* + 1 */; it != shapes.rend(); it++) {
    ImGui::PushID(j);
    ImGui::Text("Object %i", j);
    ImGui::Text("Enter a value x:");
    ImGui::SameLine(); // Place the input box on the same line as the label
    ImGui::SliderFloat("x", &((*it)->mesh->properties->vpos.x), -10.0f, 10.0f);
    ImGui::Text("Enter a value y");
    ImGui::SameLine(); // Place the input box on the same line as the label
    ImGui::SliderFloat("y", &((*it)->mesh->properties->vpos.y), -10.0f, 10.0f);
    ImGui::Text("Enter a value z:");
    ImGui::SameLine(); // Place the input box on the same line as the label
    ImGui::SliderFloat("z", &((*it)->mesh->properties->vpos.z), -10.0f, 10.0f);
    ImGui::Spacing();
    ImGui::Text("vvelocity.x: %.3f",
                static_cast<float>((*it)->mesh->properties->vvelocity.x));
    ImGui::Text("vvelocity.y: %.3f",
                static_cast<float>((*it)->mesh->properties->vvelocity.y));
    // ImGui::Text("vprevPos.x: %.3f",
    //             static_cast<float>((*it)->mesh->properties->vprevPos.x));
    // ImGui::Text("vprevPos.y: %.3f",
    //             static_cast<float>((*it)->mesh->properties->vprevPos.y));
    ImGui::PopID();
    j++;
  }
};
