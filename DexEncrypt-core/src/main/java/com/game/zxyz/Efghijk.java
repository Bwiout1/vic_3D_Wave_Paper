package com.game.zxyz;

import androidx.annotation.Keep;

import com.game.crypt.cryptolib.ConcurrencyEncDecHelper;
import com.game.crypt.cryptolib.CryptoUtil;
import com.game.crypt.cryptolib.EncDecHelper;
import com.game.crypt.cryptolib.crypto.DiscreteMapper;
import com.game.crypt.cryptolib.exception.InvalidArgumentsException;
import com.game.crypt.cryptolib.exception.NoSpaceException;
import com.game.crypt.cryptolib.noise.InvalidNoiseDataException;
import com.game.crypt.cryptolib.noise.NoiseProcessor;
import com.game.crypt.cryptolib.util.HexStrUtil;

import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.concurrent.ExecutionException;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;

/**
 * @Description 将暴露给其他module的方法集中于此,方便统一keep
 * @Date 2023/5/22 13:22
 * @Created by xmas
 */

//todo:修改类名与dota.core
@Keep
public class Efghijk {

    private final static Efghijk INSTANCE = new Efghijk();

    public static Efghijk getInstance(){
        return INSTANCE;
    }

    public static final String[] a = EncDecHelper.CIPHERS;

    public static byte[] b(String key, byte[] data) throws InvalidAlgorithmParameterException, NoSuchPaddingException, NoSuchAlgorithmException, ExecutionException, InvalidKeyException, InterruptedException, IllegalBlockSizeException, BadPaddingException, InvalidArgumentsException, NoSpaceException {
        return ConcurrencyEncDecHelper.decAndRemovePadding(key, data);
    }

    public static int c(String key) {
        return EncDecHelper.getCipherIndexByKey(key);
    }

    public long d(String s)  {
        return DiscreteMapper.getInstance().map2Long(s);
    }

    public byte[] e(byte[] s) {
        return DiscreteMapper.getInstance().map2Bytes(s);
    }

    public byte[] f(String key, byte[] noisedData) throws InvalidNoiseDataException {
        NoiseProcessor np = new NoiseProcessor(d(key));
        return np.removeNoise(noisedData);
    }

    public static String g(byte[] byteArray) {
        return HexStrUtil.toHexString(byteArray);
    }

    public static byte[] h(String hexString) {
        return HexStrUtil.toByteArray(hexString);
    }

    public static byte[] i(String key, byte[] in) throws InvalidAlgorithmParameterException, NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException, IllegalBlockSizeException, BadPaddingException, InvalidArgumentsException {
        return CryptoUtil.decAndRemovePadding(key, in);
    }
}
