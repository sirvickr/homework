#pragma once

DWORD Encrypt(LPBYTE pbContent, DWORD cbContent, LPBYTE *ppbEncodedBlob, LPDWORD pcbEncodedBlob);
DWORD Decrypt(LPBYTE* ppbContent, LPDWORD pcbContent, LPBYTE pbEncodedBlob, DWORD cbEncodedBlob);
