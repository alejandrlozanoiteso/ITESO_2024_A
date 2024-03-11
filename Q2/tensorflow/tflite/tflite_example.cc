#include <fstream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/string_util.h>
#include <tensorflow/lite/examples/label_image/get_top_n.h>
#include <tensorflow/lite/model.h>


int main(int argc, char **argv)
{

    // Get Model label and input image

    const char *modelFileName = argv[1];
    // Load Model
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(modelFileName);
    if (model == nullptr)
    {
        fprintf(stderr, "failed to load model\n");
        exit(-1);
    }
    // Initiate Interpreter
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder(*model.get(), resolver)(&interpreter);
    if (interpreter == nullptr)
    {
        fprintf(stderr, "Failed to initiate the interpreter\n");
        exit(-1);
    }

    if (interpreter->AllocateTensors() != kTfLiteOk)
    {
        fprintf(stderr, "Failed to allocate tensor\n");
        exit(-1);
    }

    printf("0x%x\n",interpreter->typed_input_tensor<float32_t>(0));

    // Configure the interpreter
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(1);
    // Get Input Tensor Dimensions
    int input = interpreter->inputs()[0];
    auto dimenssion = interpreter->tensor(input)->dims->data[1];
    std::cout << "Dimenssion " << dimenssion << std::endl;


    float32_t celcius;
    sscanf(argv[2], "%f", &celcius);
    // Copy data to input tensor
    memcpy(interpreter->typed_input_tensor<float32_t>(0), &celcius, sizeof(float32_t));
    
    // Inference
    interpreter->Invoke();

    // Get Output
    int output = interpreter->outputs()[0];
    TfLiteIntArray *output_dims = interpreter->tensor(output)->dims;
    auto output_size = output_dims->data[output_dims->size - 1];

    std::cout << "output_size " << output_size  << std::endl;   
    std::cout << "Data " << interpreter->typed_output_tensor<float32_t>(0)[0] << std::endl; 

    return 0;
}