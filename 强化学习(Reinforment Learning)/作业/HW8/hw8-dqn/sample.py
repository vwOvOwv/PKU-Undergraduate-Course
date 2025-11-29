from typing import Any, Iterator, Set, List
from typing_extensions import Self

from numbers import Number
import numpy as np
import torch

'''
Frame是一帧样本或者一帧观测,作为Agent类predict和exploit的参数
对于简单环境，FrameArrayNumpy/FrameArrayTorch是无字典结构的帧，直接继承自ndarray和torch tensor
对于复杂环境，FrameDict是带字典结构的一帧,可以有FrameNumpy/FrameTorch形态

SampleBatch是多帧数据的合集，一定拥有字典结构（作为样本）
SampleBatchNumpy/SampleBatchTorch是不同形态,继承自SampleBatch

FrameNumpy从嵌套字典进行初始化，通过调用FrameNumpy.from_dict
SampleBatchNumpy通过合并FrameNumpy列表进行初始化，调用SampleBatch.stack
'''


def _is_number(value: Any) -> bool:
    return isinstance(value, Number) or isinstance(value, np.number) or isinstance(value, np.bool_)

def flatten_dict(d: dict, suffix: str = '') -> dict:
    res = {}
    for key, value in d.items():
        if suffix: key = '%s.%s' % (suffix, key)
        if isinstance(value, dict):
            res.update(flatten_dict(value, key))
        elif _is_number(value) or isinstance(value, np.ndarray):
            res[key] = value
        else:
            raise ValueError('Only support number and ndarray, but found type %s under key %s!' % (type(value), key))
    return res

class Frame:

    @classmethod
    def convert(cls, data):
        if isinstance(data, Frame):
            return data
        elif isinstance(data, np.ndarray):
            return data.view(FrameArrayNumpy)
        elif isinstance(data, dict):
            return FrameNumpy.from_dict(data)
        else:
            raise ValueError('Data should be np.ndarray or dict, but found %s!' % type(data))

    def to_torch(
        self,
        dtype: 'torch.dtype | None' = None,
        device: 'str | int | torch.device' = "cpu"
    ):
        raise NotImplementedError

class FrameDict(Frame):

    def __init__(self, flatten_d):
        flag_torch = False
        for key, value in flatten_d.items():
            if isinstance(value, torch.Tensor):
                flag_torch = True
                break
            elif _is_number(value) or isinstance(value, np.ndarray):
                continue
            else:
                raise ValueError('Found unsupported value type %s under key %s!' % (type(value), key))
        if flag_torch:
            return FrameTorch(flatten_d)
        else:
            return FrameNumpy(flatten_d)
    
    def keys(self) -> Set[str]:
        keys = [k.split('.')[0] for k in self._data.keys()]
        return set(keys)
    
    def _get_key(self, key: str) -> Any:
        if key in self._data:
            # key索引已到叶子节点
            value = self._data[key]
            if isinstance(value, np.ndarray):
                return value.view(FrameArrayNumpy)
            elif isinstance(value, torch.Tensor):
                return FrameArrayTorch(value)
            else:
                return value
        keys = [k for k in self._data.keys() if k.startswith(key + '.')]
        if keys:
            t = len(key) + 1
            data = {key[t:] : self._data[key] for key in keys}
            return type(self)(data)
        return None

    def __getattr__(self, key: str) -> Any:
        data = self._get_key(key)
        if data is None:
            raise KeyError("Key %s not exist!" % key)
        return data

    def __getitem__(self, index: 'str') -> Any:
        if isinstance(index, str):
            data = self._get_key(index)
            if data is None:
                raise IndexError("Key %s not exist!" % index)
            return data
        else:
            raise IndexError("Index should be str or int or slice, found %s!" % type(index))

    def to_torch(
        self,
        dtype: 'torch.dtype | None' = None,
        device: 'str | int | torch.device' = "cpu"
    ):
        data = {}
        for key, value in self._data.items():
            if isinstance(value, np.ndarray):
                value = torch.from_numpy(value).to(device)
                if dtype: value = value.type(dtype)
            elif isinstance(value, torch.Tensor):
                value = value.to(device)
                if dtype: value = value.type(dtype)
            # value is number: do not change
            data[key] = value
        return FrameTorch(data)


class FrameNumpy(FrameDict):

    def __init__(self, flatten_d):
        self._data = flatten_d

    @classmethod
    def from_dict(cls, d):
        return cls(flatten_dict(d))


class FrameTorch(FrameDict):

    def __init__(self, flatten_d):
        self._data = flatten_d



class FrameArrayNumpy(np.ndarray, Frame):

    def offset(self, shift: int):
        return np.roll(self, shift, 0)
    
    def to_torch(
        self,
        dtype: 'torch.dtype | None' = None,
        device: 'str | int | torch.device' = "cpu"
    ):
        data = torch.from_numpy(self).to(device)
        if dtype: data = data.type(dtype)
        return data
        

class FrameArrayTorch(torch.Tensor, Frame):

    def offset(self, shift: int):
        return torch.roll(self, shift, 0)
    
    def to_torch(
        self,
        dtype: 'torch.dtype | None' = None,
        device: 'str | int | torch.device' = "cpu"
    ):
        data = self.to(device)
        if dtype: data = data.type(dtype)
        return data


class SampleBatch:

    def __len__(self) -> int:
        return self._length
    
    def keys(self) -> Set[str]:
        keys = [k.split('.')[0] for k in self._data.keys()]
        return set(keys)
    
    def _get_key(self, key: str) -> Any:
        if key in self._data:
            # key索引已到叶子节点
            value = self._data[key]
            if isinstance(value, np.ndarray):
                return value.view(FrameArrayNumpy)
            elif isinstance(value, torch.Tensor):
                return FrameArrayTorch(value)
            else:
                return value
        keys = [k for k in self._data.keys() if k.startswith(key + '.')]
        if keys:
            t = len(key) + 1
            data = {key[t:] : self._data[key] for key in keys}
            return type(self)(data)
        return None

    def __getattr__(self, key: str) -> Any:
        data = self._get_key(key)
        if data is None:
            raise KeyError("Key %s not exist!" % key)
        return data

    def __getitem__(self, index: 'str | slice | int') -> Any:
        if isinstance(index, str):
            # 基于字典的索引
            data = self._get_key(index)
            if data is None:
                raise IndexError("Key %s not exist!" % index)
            return data
        elif isinstance(index, int):
            # 基于下标的索引
            if not self._data:
                raise IndexError("Empty sample!")
            data = {key : value[index] for key, value in self._data.items()}
            return FrameDict(data)
        elif isinstance(index, slice):
            # 基于分片的索引
            if not self._data:
                raise IndexError("Empty sample!")
            data = {key : value[index] for key, value in self._data.items()}
            length = len(range(self._length)[index])
            return type(self)(data, length)
        else:
            raise IndexError("Index should be str or int or slice, found %s!" % type(index))

    def __iter__(self) -> Iterator[Self]:
        if self._length == 0:
            yield from []
        else:
            for i in range(self._length):
                yield self[i]

    def to_torch(
        self,
        dtype: 'torch.dtype | None' = None,
        device: 'str | int | torch.device' = "cpu"
    ):
        data = {}
        for key, value in self._data.items():
            if isinstance(value, np.ndarray):
                value = torch.from_numpy(value).to(device)
                if dtype: value = value.type(dtype)
            elif isinstance(value, torch.Tensor):
                value = value.to(device)
                if dtype: value = value.type(dtype)
            # value is number: do not change
            data[key] = value
        return SampleBatchTorch(data, self._length)
    
    def offset(self, shift: int) -> Self:
        raise NotImplementedError


class SampleBatchNumpy(SampleBatch):

    def __init__(self, flatten_d: dict = {}, length : int = 0):
        self._data = flatten_d
        self._length = length

    def _set_key(self, key: str, value: 'np.ndarray | Self') -> None:
        # 删掉现有的键值
        if key in self._data:
            del self._data[key]
        keys = [k for k in self._data.keys() if k.startswith(key + '.')]
        for key in keys:
            del self._data[key]
        # 赋值
        if isinstance(value, np.ndarray):
            if value.shape[0] != self._length:
                raise ValueError('Incompatible length when setting sample[%s], sample length %d, value length %d' % (key, self._length, value.shape[0]))
            self._data[key] = value
        elif isinstance(value, SampleBatchNumpy):
            for k, v in value._data.items():
                if v.shape[0] != self._length:
                    raise ValueError('Incompatible length when setting sample[%s], sample length %d, value[%s] length %d' % (key, self._length, k, value.shape[0]))
                self._data['%s.%s' % (key, k)] = v
    
    def __setattr__(self, key: str, value: 'np.ndarray | Self') -> None:
        if key == '_data' or key == '_length':
            super(SampleBatchNumpy, self).__setattr__(key, value)
        else:
            self._set_key(key, value)
    
    def __setitem__(self, index: 'str | slice | int', value: 'np.ndarray | FrameNumpy | Self') -> None:
        if isinstance(index, str):
            # 基于字典的索引
            self._set_key(index, value)
        elif isinstance(index, int):
            # 基于下标的索引
            if not isinstance(value, FrameNumpy):
                raise ValueError('Value should be FrameNumpy, not %s!' % type(value))
            raise NotImplementedError('Do not support assigning to index yet.')
        elif isinstance(index, slice):
            # 基于分片的索引
            if not isinstance(value, SampleBatchNumpy):
                raise ValueError('Value should be SampleBatchNumpy, not %s!' % type(value))
            raise NotImplementedError('Do not support assigning to slice yet.')
        else:
            raise IndexError("Index should be str or int or slice, found %s!" % type(index))
    
    @classmethod
    def stack(cls, samples: List[FrameNumpy]) -> Self:
        length = len(samples)
        if length == 0:
            return cls()
        keys = set.intersection(*(set(sample._data.keys()) for sample in samples))
        data = {key : np.stack([sample._data[key] for sample in samples]) for key in keys}
        return cls(data, length)
    
    @classmethod
    def concat(cls, samples: List[Self], axis = 0) -> Self:
        if len(samples) == 0:
            return cls()
        keys = set.intersection(*(set(sample._data.keys()) for sample in samples))
        data = {key : np.concatenate([sample._data[key] for sample in samples], axis = axis) for key in keys}
        length = sum([sample._length for sample in samples])
        return cls(data, length)

    def offset(self, shift: int) -> Self:
        data = {key: np.roll(value, shift, 0) for key, value in self._data}
        length = self._length
        return type(self)(data, length)

class SampleBatchTorch(SampleBatch):

    def __init__(self, flatten_d: dict, length: int):
        self._data = flatten_d
        self._length = length
    
    def offset(self, shift: int) -> Self:
        data = {key: torch.roll(value, shift, 0) for key, value in self._data}
        length = self._length
        return type(self)(data, length)