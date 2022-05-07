<template>
  <div class="container-sm" style="padding-top: 20vh">
    <div class="row justify-content-md-center">
      <div class="col col-lg-2"></div>
      <div class="col-md-auto">
        <div class="card center">
          <div class="card-body">
            <h1 class="card-title" style="color: black; font-size: 8rem">
              {{ msg }}
            </h1>

            <div class="input-group flex-nowrap">
              <input
                class="form-control"
                type="text"
                v-model="send"
                placeholder="Type here"
                :disabled="isSubmit"
                @keydown.enter="Emit"
              />

              <button
                :disabled="isSubmit"
                type="button"
                class="btn btn-secondary"
                @click="Emit"
              >
                {{ btnText }}
                <div
                  v-if="isSubmit"
                  class="spinner-border spinner-border-sm"
                  role="status"
                >
                  <span class="visually-hidden">Loading...</span>
                </div>
              </button>
            </div>
          </div>
        </div>
      </div>
      <div class="col col-lg-2"></div>
    </div>

    <div
      v-show="toast.show"
      class="position-fixed bottom-0 end-0 p-3"
      style="z-index: 11"
    >
      <div :class="toast.class">
        <div class="toast-header">
          <strong class="me-auto">{{ toast.header }}</strong>
          <small>Just now</small>
          <button
            type="button"
            class="btn-close"
            data-bs-dismiss="toast"
          ></button>
        </div>
        <div class="toast-body">
          <p>{{ toast.msg }}</p>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  head() {
    return {
      title: this.title,
      meta: [
        {
          hid: 'IoT',
          name: 'IoT',
          content: 'IoT',
        },
      ],
    }
  },
  data() {
    return {
      msg: 'IoT',
      isSubmit: false,
      btnText: 'Submit',
      title: 'IoT Main',
      send: '',
      wsConnection: null,
      toast: {
        show: false,
        header: '',
        msg: '',
        class: '',
      },
    }
  },
  created() {
    this.wsConnection = new WebSocket(process.env.wsHost)

    this.wsConnection.addEventListener('close', async () => {
      this.toast = {
        show: true,
        header: 'Error',
        msg: 'Disconnected from Web Socket Server.',
        class: 'toast show text-white bg-danger border-0',
      }
      await this.delay(2)
      this.resetToast()
      this.reconnect()
    })

    this.wsConnection.addEventListener('open', async () => {
      this.toast = {
        show: true,
        header: 'Success',
        msg: 'Connected to Web Socket Server.',
        class: 'toast show text-white bg-success border-0',
      }
      this.wsConnection.send(
        JSON.stringify({
          latest: true,
        })
      )
      await this.delay(1)
      this.resetToast()
    })

    this.wsConnection.addEventListener('message', async (msg) => {
      const data = JSON.parse(msg.data)
      const { received, msgs, broadcast } = data
      if (received) {
        console.log("I'm the sender.")
      }
      if (broadcast) {
        console.log("I'm the receiver.")
      }
      if (!msgs) return

      this.msg = msgs
    })
  },
  methods: {
    reconnect() {
      console.log('Reconnect to WebSocket Server.')
      this.wsConnection = new WebSocket(process.env.wsHost)
    },
    async Emit() {
      this.btnText = 'Loading'
      this.isSubmit = true

      await this.delay(2)
      const data = {
        msg: this.send,
        isEmit: true,
      }
      this.wsConnection.send(JSON.stringify(data))
      this.toast = {
        show: true,
        header: 'Success',
        msg: 'Emit message successfully.',
        class: 'toast show text-white bg-primary border-0',
      }
      await this.reset()
    },
    resetToast() {
      this.toast = {}
    },
    async reset() {
      this.btnText = 'Submit'
      this.send = ''
      this.isSubmit = false
      await this.delay(2)
      this.resetToast()
    },
    async delay(sec) {
      return new Promise((resolve) => setTimeout(resolve, sec * 1000))
    },
  },
}
</script>

<style scoped>
.center {
  width: 50rem;
  box-shadow: 0px 8px 18px -19px black;
  text-align: center;
}
</style>
