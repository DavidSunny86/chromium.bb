Polymer({

    is: 'transform-animation',

    behaviors: [
      Polymer.NeonAnimationBehavior
    ],

    /**
     * @param {{
     *   node: !Element,
     *   transformOrigin: (string|undefined),
     *   transformFrom: (string|undefined),
     *   transformTo: (string|undefined),
     *   timing: (Object|undefined)
     * }} config
     */
    configure: function(config) {
      var node = config.node;
      var transformFrom = config.transformFrom || 'none';
      var transformTo = config.transformTo || 'none';

      if (config.transformOrigin) {
        this.setPrefixedProperty(node, 'transformOrigin', config.transformOrigin);
      }

      this._effect = new KeyframeEffect(node, [
        {'transform': transformFrom},
        {'transform': transformTo}
      ], this.timingFromConfig(config));

      return this._effect;
    }

  });