var config = {
    apiUrl: "/api/",
    mainnetExplorerUrl: "https://blocks.arqma.com/",
    testnetExplorerUrl: "",
    stagenetExplorerUrl: "",
    nettype: 0, /* 0 - MAINNET, 1 - TESTNET, 2 - STAGENET */
    coinUnitPlaces: 9,
    txMinConfirms: 4,         // corresponds to CRYPTONOTE_DEFAULT_TX_SPENDABLE_AGE in Monero
    txCoinbaseMinConfirms: 18, // corresponds to CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW in Monero
    coinSymbol: 'ARQ',
    openAliasPrefix: "arq",
    coinName: 'ArQmA',
    coinUriPrefix: 'arqma:',
    addressPrefix: 0x2cca,
    integratedAddressPrefix: 0x116bc7,
    subAddressPrefix: 0x6847,
    addressPrefixTestnet: 0x53ca,
    integratedAddressPrefixTestnet: 0x504a,
    subAddressPrefixTestnet: 0x524a,
    addressPrefixStagenet: 0x39ca,
    integratedAddressPrefixStagenet: 0x1742ca,
    subAddressPrefixStagenet: 0x1d84ca,
    feePerKB: new JSBigInt('10000'),//20^10 - not used anymore, as fee is dynamic.
    dustThreshold: new JSBigInt('10000'),//10^10 used for choosing outputs/change - we decompose all the way down if the receiver wants now regardless of threshold
    txChargeRatio: 0.8,
    defaultMixin: 6, // minimum mixin for hardfork v7 is 6 (ring size 7)
    txChargeAddress: 'ar46iCiw5uB7SjnYhL5EJLP1LpqGkZbCcWhWgdbLL1c4DicNuYi3ZeRJPi8FFmEhYnagbxRyaQKyTYBA95JqmPcr1XZytK9o3',
    idleTimeout: 30,
    idleWarningDuration: 20,
    maxBlockNumber: 500000000,
    avgBlockTime: 120,
    debugMode: false
};
