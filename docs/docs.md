# To be clear I slowly but succesfuly making thit documentation, so it can contain not every piece of information you want get!!! 



## That`s a table of content documentation of engine
- [qe::Renderer](all_docs_md/renderer.md)
- [qe::Window](all_docs_md/window.md)
- [qe::Time](all_docs_md/time.md)
- [qe::Layers](all_docs_md/layers.md)

### void Application()

    Description:
        void Appliation() is entry point of engine, so DO NOT ADD int main() to code!!!!!

    Use example:
       
       ```c++
       void Application() {
           printf("Hello world from Quintus Engine!");
       }
       ```

#### Scene structure
    0x01 0x01 0x00 0x02 0x01 0x00 "Scene name" 0x00 0x02 0x02 0x00 0x03 0x01 0x00 "Var name" 0x00 0x03 0x02 0x00 0x04 0x01 0x00 "Variable values in string" 0x00 0x04 0x02 0x00 0x01 0x02 0x00
    
