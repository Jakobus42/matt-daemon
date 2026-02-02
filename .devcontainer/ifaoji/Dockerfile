FROM ghcr.io/jakobus42/clang-p2996-dev:latest

USER root

ADD --checksum=sha256:c9ff7ee3113f039c21c3ff31e8e2b71d1cc51a2c3e6c39face36e3cf25fe8788 https://github.com/helix-editor/helix/releases/download/25.07.1/helix_25.7.1-1_amd64.deb /tmp/helix.deb
SHELL ["/bin/zsh", "-c"]
RUN apt-get update && apt-get install -y /tmp/helix.deb && rm /tmp/helix.deb

RUN apt-get install -y openssh-client

# Ubuntu 24 creates a default user `ubuntu`, we no want that
RUN if id -u ubuntu >/dev/null 2>&1; then userdel -r ubuntu; fi && \
    useradd -m -u 1000 -s /bin/bash dev

RUN apt-get update && apt-get install -y sudo \
    && echo "dev ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/dev \
    && chmod 0440 /etc/sudoers.d/dev

RUN chown -R dev:dev /home/dev

USER dev
WORKDIR /home/dev

ENV EDITOR=hx

CMD ["/bin/bash"]
