#ifndef OWN_SYSTEM_H
#define OWN_SYSTEM_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>

namespace own_types {
    enum class Types{
        INT, BOOL, CELL,
    };
    enum class CELL_VALUE {
        UNDEFINED = 0,
        EMPTY,
        WALL,
        EXIT,
    };
    union val {
        int Ival;
        bool Bval;
        CELL_VALUE Cval;
        explicit val(int value) :Ival(value) {};
        explicit val(bool value) :Bval(value) {};
        explicit val(CELL_VALUE value) :Cval(value) {};
    };
    struct value {
        Types t;
        val v;
        explicit value(int v): t(Types::INT), v(v) {};
        explicit value(bool v): t(Types::BOOL), v(v) {};
        explicit value(CELL_VALUE v): t(Types::CELL), v(v) {};
    };
    
    

    template <class T>
    struct Array {
        std::vector<T> data;
        Array(size_t size, const T& startValue): data(size, startValue) {}
        Array(const Array<T>& o): data(o.data) {}
        T& operator[](size_t i) {
            return data[i];
        }
        const T& operator[](size_t i) const {
            return data[i];
        }          

        Array<T>& operator=(const Array<T>& o) {
            data = o.data;
            return *this;
        }
        friend std::ostream& operator<<(std::ostream& out, const Array<T>& printable) {
            out << "{";
            for (auto& i: printable.data) {
                out << i << ",";
            }
            out << "}";
            return out;
        }
    };
    
    template <class T>
    struct subarray : public Array<T> {
        subarray(size_t size, const T& startValue): Array<T>(size, startValue) {}
        subarray(const Array<T>& o): Array<T>(o) {}

        subarray<T>& operator=(const Array<T>& o) {
            if (this->data.size() != o.data.size()) 
                throw std::invalid_argument("Bad array size:" + std::to_string(this->data.size()) + " " +  std::to_string(o.data.size()));
            for (size_t i = 0; i < this->data.size(); i++) {
                this->data[i] = o.data[i];
            }
            return *this;
        }
    };

    template <typename T>
    class MultiArray {
    private:
        std::vector<size_t> dimensions_;    // Размеры каждого измерения
        std::vector<T> data_;               // Хранилище данных (одномерный вектор)
    public:
        /// @brief конструктор по размерности
        /// @param dimensions массив максимальных индексов
        MultiArray(const std::vector<size_t>& dimensions) : dimensions_(dimensions) {
            size_t totalSize = 1;
            for (size_t dim : dimensions_) {
                totalSize *= dim;
            }
            data_.resize(totalSize);
        }

        T& operator[](const std::vector<size_t>& indices) {
            if (indices.size() != dimensions_.size())
                throw std::invalid_argument("Too few indexes");
            return *sub_begin(indices);
        }
        const T& operator[](const std::vector<size_t>& indices) const {
            if (indices.size() != dimensions_.size())
                throw std::invalid_argument("Too few indexes");
            return *sub_begin(indices);
        }
        
        /// @brief вычисление размера вложенного подмассива
        /// @param deep "глубина" подмассива
        size_t calculate_sub_size(size_t deep) {
            if (deep > dimensions_.size())
                throw std::invalid_argument("Too deep");
            size_t sub_size = 1;
            for (size_t i = deep; i < dimensions_.size(); i++)
                sub_size *= dimensions_[i];
            return sub_size;
        }
        /// @brief получить размерность подмассива
        /// @param deep "глубина" подмассива
        /// @return массив максимальных индексов
        std::vector<size_t> get_sub_dimentions(size_t deep) {
            return std::vector(dimensions_.begin()+deep, dimensions_.end());
        }
        /// @brief получение итератора на начало подмассива
        /// @param indexes координаты подмассива
        auto sub_begin(const std::vector<size_t>& indexes) {
            size_t sub_size = calculate_sub_size(indexes.size());
            for (size_t i = 0; i < indexes.size(); i++) {
                if (indexes[i] >= dimensions_[i])
                    throw std::invalid_argument("Index overflow at " + std::to_string(i) + " dimention;" +std::to_string(indexes[i]) +" more than "+ std::to_string(dimensions_[i]));
            }
            size_t begin_ind = 0;
            size_t s = sub_size;
            for (size_t i = indexes.size(); i != 0; --i) {
                begin_ind += s*indexes[i-1];
                s *= dimensions_[i-1];
            }
            return data_.begin() + begin_ind;
        }
        /// @brief получение итератора на конец подмассива
        /// @param indexes координаты подмассива
        auto sub_end(const std::vector<size_t>& indexes) {
            size_t sub_size = calculate_sub_size(indexes.size());
            for (size_t i = 0; i < indexes.size(); i++) {
                if (indexes[i] >= dimensions_[i])
                    throw std::invalid_argument("Index overflow: " + std::to_string(i) + " dimention");
            }
            size_t begin_ind = 0;
            size_t s = sub_size;
            for (size_t i = indexes.size(); i != 0; i--) {
                begin_ind += s*indexes[i-1];
                s *= dimensions_[i-1];
            }
            return data_.begin() + begin_ind + sub_size;
        }

        /// @brief устанавливить подмассив
        /// @param indexes координаты
        /// @param sub новые значения
        void setSubArray(const std::vector<size_t>& indexes, const MultiArray<T>& sub) {
            if (indexes.size() > dimensions_.size())
                throw std::invalid_argument("Too many indexes");
            if (!std::equal(dimensions_.begin() + indexes.size(), dimensions_.end(), sub.dimensions_.begin(), sub.dimensions_.end()))
                throw std::invalid_argument("Wrong subarray size");
            
            std::copy(sub.data_.begin(), sub.data_.end(), sub_begin(indexes));
        }
        /// @brief получить копию подмассива
        /// @param indexes координаты
        MultiArray<T> copySubArray(const std::vector<size_t>& indexes) {
            auto b = sub_begin(indexes);
            auto n_dim = std::vector<size_t>(dimensions_.begin()+indexes.size(), dimensions_.end());
            
            MultiArray<T> res(n_dim);
            std::copy(b, sub_end(indexes), res.data_.begin());
            return res;
        }
    };
} // namespace own_types
#endif