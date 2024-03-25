package com.game.crypt.cryptolib.crypto;

import com.game.crypt.cryptolib.exception.InvalidArgumentsException;

import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;

import javax.crypto.Cipher;
import javax.crypto.NoSuchPaddingException;

public interface ICipherBuilder {
    void init(String cipherStr, String key, boolean isEnc) throws InvalidArgumentsException;
    byte[] getKey1();
    byte[] getKey2();
    Cipher build() throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException,
            InvalidAlgorithmParameterException;
}
