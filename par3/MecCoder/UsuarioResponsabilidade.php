<?php
namespace Simec\Par3;
    /**
     * Classe que determina a responsabilidade do usuário
     */
    class UsuarioResponsabilidade implements \Simec\Corporativo\Usuario\ResponsabilidadeInterface
    {

        /**
         * Retorna a abrangência da responsabilidade do usuário
         * @return array
         */
        public function abrangencia()
        {
            $res = pegarResponsabilidadeUsuario($_SESSION['usucpf']);
            $responsabilidades = [];
            if (isset($res['muncod']) && $res['muncod']) {
                $responsabilidades['muncod'] = $res['muncod'];
                $responsabilidades['municipios'] = (new \Territorios_Model_Municipio())->pegarMunicipios($res['muncod']);
            }
            if (isset($res['estuf']) && $res['estuf']) {
                $responsabilidades['estuf'] = $res['estuf'];
                $responsabilidades['estados'] = (new \Territorios_Model_Estado())->pegarEstadosPorSigla($res['estuf']);
            }
            return $responsabilidades;
        }
    }
