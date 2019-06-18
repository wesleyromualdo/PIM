<?php
/**
 * Classe de mapeamento da entidade par3.cae_conselheiro.
 *
 * $Id$
 */

/**
 * Mapeamento da entidade par3.cae_conselheiro.
 *
 * @see Modelo
 */
class Par3_Model_ConselhoCAE extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'integracao.s_conselho_cae';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'co_municipio_ibge' => null ,
        'ds_situacao_mandato' => null,
        'sg_uf' => null,
        'no_municipio' => null,
        'ds_email_conselho' => null,
        'nu_cpf_membro' => null,
        'no_membro character' => null,
        'tp_tipo_nomeacao' => null,
        'nu_numero_ato' => null,
        'ds_email_membro_conselho' => null,
        'ds_categoria_membro_conselho' => null,
        'dt_inicio_mandato' => null,
        'tp_membro' => null,
        'no_cargo' => null
    );

    /**
     * Função retonarResponsavelCAE
     * - retorna array de entidades do tipo conselheiro.
     *
     * @return array $arrDados.
     *
     */
    public function retonarResponsavelCAE($estuf, $muncod = null)
    {
        $sql = "SELECT  co_municipio_ibge as muncod,
                        ds_email_conselho as entemail,
                        'CAE - ' || no_municipio as entnome,
                        nu_numero_ato as caedocumento,
                        dt_inicio_mandato as caedtdocumento,
                        nu_cpf_membro as caerespcpf,
                        no_membro as caerespnome,
                        no_cargo as entcargo,
                        CASE
                          WHEN tp_tipo_nomeacao = 'DECRETO MUNICIPAL' THEN 'D'
                          WHEN tp_tipo_nomeacao = 'DECRETO ESTADUAL' THEN 'D'
                          WHEN tp_tipo_nomeacao = 'PORTARIA' THEN 'P'
                          WHEN tp_tipo_nomeacao = 'ATOS' THEN 'O'
                          ELSE 'O'
                        END as caenomeacao,
                        CASE
                          WHEN ds_situacao_mandato = 'VÁLIDO' THEN 'A'
                          WHEN ds_situacao_mandato = 'DILIGENCIADO' THEN 'D'
                          WHEN ds_situacao_mandato = 'EM ANÁLISE' THEN 'E'
                          ELSE 'I'
                        END as caestatus
		    	FROM 	integracao.s_conselho_cae
		    	WHERE 	no_cargo = 'PRESIDENTE(A)'
		    	  AND	sg_uf = '{$estuf}'";

        if ($muncod) {
            $sql.= " AND co_municipio_ibge = '{$muncod}'";
        }

        return adapterConnection::coorporativo()->pegaLinha($sql);
    }//end retonarResponsavelCAE()]

    
    /**
     * Função retonarConselheirosCAE
     * - retorna array de entidades do tipo conselheiro.
     *
     * @return array $arrDados.
     *
     */
    public function retonarConselheirosCAE($estuf, $muncod = null)
    {
        $sql = "SELECT  co_municipio_ibge as cacfuncao,
                        ds_email_conselho as cacmandato,
                        'CAE - ' || no_municipio as cacocupacao,
                        nu_numero_ato as cacrenda,
                        dt_inicio_mandato as cacoutroconselho,
                        nu_cpf_membro as cacregime,
                        no_membro as cacplanoacao,
                        no_cargo as cacreuniao,
                        as caccapacitacao,
                        as cacorgaocapacitacao,
                        as cacorgaocapacitacaooutro,
                        as cacpopulacaoindigena,
                        as cacrepresentanteoindigena
                        CASE
                          WHEN tp_tipo_nomeacao = 'DECRETO MUNICIPAL' THEN 'D'
                          WHEN tp_tipo_nomeacao = 'DECRETO ESTADUAL' THEN 'D'
                          WHEN tp_tipo_nomeacao = 'PORTARIA' THEN 'P'
                          WHEN tp_tipo_nomeacao = 'ATOS' THEN 'O'
                          ELSE 'O'
                        END as caenomeacao,
                        CASE
                          WHEN ds_situacao_mandato = 'VÁLIDO' THEN 'A'
                          WHEN ds_situacao_mandato = 'DILIGENCIADO' THEN 'D'
                          WHEN ds_situacao_mandato = 'EM ANÁLISE' THEN 'E'
                          ELSE 'I'
                        END as caestatus
		    	FROM 	integracao.s_conselho_cae
		    	WHERE 	no_cargo <> 'PRESIDENTE(A)'
		    	  AND	sg_uf = '{$estuf}'";

        if ($muncod) {
            $sql.= " AND co_municipio_ibge = '{$muncod}'";
        }

        return adapterConnection::coorporativo()->carregar($sql);
    }//end retonarConselheirosCAE()]

}//end Class
?>