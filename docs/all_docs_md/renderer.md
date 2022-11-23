# qe::Renderer class documentation

qe::Renderer needs to be added to LayerHandler in [qe::Window](window.md) class 

# TODO: Renderer documentation

## qe::Renderer functions
------------

/**
         * @brief Restart (reattach) all shaders 
         * 
         */
        void ReattachShaders() {

        /**
         * @brief Add shader id to shaders heap
         * 
         * @param shader_id 
         */
        void AddShader(uint32_t shader_id) {

        /**
         * @brief Clear all shaders from heap
         * 
         */
        void ClearShaders() {

        /**
         * @brief Add texture to textures heap
         * 
         * @param name 
         */
        void AddTexture(const std::string name) {

        /**
         * @brief Remove texture from texture heap by id
         * 
         * @param id 
         */
        void RemoveTexture(uint32_t id) {

        void SetModelTexture(uint32_t model_id, uint32_t texture_id) {

        /**
         * @brief Set the Position object
         * 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetPosition(float x, float y, float z) {
        /**
         * @brief Set the Scale object
         * 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetScale(float x, float y, float z) {

        /**
         * @brief Set the Rotation object
         * 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetRotation(float x, float y, float z) {

        /**
         * @brief Set the Position By ID
         * 
         * @param id 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetPositionByID(uint32_t id, float x, float y, float z) {

        /**
         * @brief Set the Scale By ID
         * 
         * @param id 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetScaleByID(uint32_t id, float x, float y, float z) {

        /**
         * @brief Set the Rotation By ID
         * 
         * @param id 
         * @param x 
         * @param y 
         * @param z 
         */
        void SetRotationByID(uint32_t id, float x, float y, float z) {

        /**
         * @brief Set the Color By ID
         * 
         * @param id 
         * @param r 
         * @param g 
         * @param b 
         * @param a 
         */
        void SetColorByID(uint32_t id, float r, float g, float b, float a) {

        /**
         * @brief Add model to heap
         * 
         * @param data 
         */
        void AddModel(RenderedData data) {

        /**
         * @brief Remove model from heap by data
         * 
         * @param data 
         */
        void RemoveModel(RenderedData data) {

        /**
         * @brief Remove model from heap by id
         * 
         * @param id 
         */
        void RemoveModel(uint32_t id) {

        /**
         * @brief Force buffers data to rejoin (may be needed in some cases to have working models)
         * 
         */
        void ForceRejoin() {

        /**
         * @brief Set the Rendered object
         * 
         * @param data 
         */
        void SetRendered(Rendered data) {

        /**
         * @brief Get the Model Position object
         * 
         * @param id model ID
         * @return glm::vec3 
         */
        math::Vector<float> GetModelPosition(uint32_t id) {

        /**
         * @brief Get the Model Scale object
         * 
         * @param id model ID
         * @return glm::vec3 
         */
        math::Vector<float> GetModelScale(uint32_t id) {
        /**
         * @brief Get the Model Rotation object
         * 
         * @param id model ID
         * @return glm::vec3 
         */
        math::Vector<float> GetModelRotation(uint32_t id) {

        /**
         * @brief Debug info on public bool variables (m_render, m_with_indices, m_triangles)
         * 
         */
        void __debug_public_info() {