#include "runner.h"

int test(LIBSSH2_SESSION *session)
{
#if LIBSSH2_ED25519
    /* set in Dockerfile */
    return test_auth_pubkey(session, 0,
                            "libssh2",
                            NULL,
                            "key_ed25519.pub",
                            "key_ed25519");
#else
    (void)session;
    return 0;
#endif
}
