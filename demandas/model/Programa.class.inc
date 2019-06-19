<?php

class Programa_Model extends Modelo
{
    protected $stNomeTabela = 'corporativo.programa';

    protected $arChavePrimaria = array(
        'co_programa',
    );

    protected $arAtributos = array(
        'co_programa' => null,
        'nu_codigo_programa' => null,
        'ds_sigla' => null,
        'ds_programa' => null,
        'ds_objetivo' => null,
        'co_situacao' => null,
        'ds_link' => null
    );

    public function listaPrograma( $arrPost )
    {
        
        $sql = $this->getSqlListagemFiltro($arrPost);

        if($arrPost['returnsql']) {
            
            return $sql;
            
        } else {
            
            $arrDados = $this->carregar($sql);
            $arrDados = $arrDados ? $arrDados : array();
            
            return $arrDados;
            
        }
        
    }

    public function getSqlListagemFiltro( $arrPost ){

        $filtro = '';
        
        if( $arrPost['nu_codigo_programa'] ){
            $filtro .= " AND pg.nu_codigo_programa = '".$arrPost['nu_codigo_programa']."' ";
        }

        if( $arrPost['ds_sigla'] ){
            $filtro .= " AND pg.ds_sigla ILIKE '%".$arrPost['ds_sigla']."%' ";
        }

        if( $arrPost['co_secretaria'] ){
            $filtro .= " AND ps.co_secretaria = '".$arrPost['co_secretaria']."' ";
        }

        if( $arrPost['co_programa'] ){
            $filtro .= " AND pg.co_programa = '".$arrPost['co_programa']."' ";
        }

        if( isset($arrPost['co_situacao']) && $arrPost['co_situacao'] == 1 ){
            $filtro .= " AND pg.co_situacao = ".$arrPost['co_situacao']." ";
        }
        
        if(isset($arrPost['co_situacao'])) 
        {
            if($arrPost['co_situacao'] != -1)
                $filtro .= " AND pg.co_situacao = ".$arrPost['co_situacao'];
        }
        else {
            $filtro .= " AND pg.co_situacao = 1 ";
        }
        //$filtro .= " AND (pg.co_situacao = ".(isset($arrPost['co_situacao']) && $arrPost['co_situacao'] == -1 ? " 1 OR pg.co_situacao = 0)" : $arrPost['co_situacao'].")");
        
        $sql = "SELECT pg.co_programa, pg.nu_codigo_programa, pg.ds_programa, pg.ds_sigla, pg.ds_objetivo, ej.enjdsc, ps.co_programa_secretaria
                FROM corporativo.programa pg, corporativo.entidades_juridica ej, corporativo.programa_secretaria ps
                WHERE ps.co_programa = pg.co_programa AND ps.co_secretaria = ej.enjid {$filtro}";


        return $sql;


    }


    public function getComboSecretarias()
    {
        $sql = "SELECT ej.enjid as codigo, ej.enjdsc as descricao FROM corporativo.entidades_juridica ej 
                INNER JOIN corporativo.programa_secretaria ps ON ps.co_secretaria = ej.enjid
        WHERE enjstatus = 1";

        return $sql;
    }

    public function getComboSecretariasTodas()
    {
        $sql = "SELECT ej.enjid as codigo, ej.enjdsc as descricao FROM corporativo.entidades_juridica ej WHERE enjstatus = 1";

        return $sql;
    }

    // public function getComboGrupoPrograma()
    // {
    //     $sql = "SELECT co_grupo_programa, COALESCE(ds_grupo_programa || ' (' || nu_codigo_grupo_programa || ')' ) as descricao  FROM corporativo.grupo_programa WHERE co_status = 1";

    //     return $sql;
    // }

    public function getProgramaPorId($co_programa)
    {
        if($co_programa)
        {
            $sql = "SELECT pg.co_programa, pg.nu_codigo_programa, pg.ds_sigla, pg.ds_programa, pg.ds_objetivo, pg.ds_link
                FROM corporativo.programa pg
                WHERE pg.co_programa = '$co_programa'";

            $arrDados = $this->pegaLinha($sql);
        }

        $arrDados = $arrDados ? $arrDados : array();
        return $arrDados;
    }

    public function getSecretariasDoPrograma($co_programa)
    {
        if($co_programa)
        {            
            $sql = "SELECT ps.co_secretaria as codigo, ej.enjdsc as descricao
                    FROM corporativo.programa_secretaria ps, corporativo.entidades_juridica ej
                    WHERE ps.co_programa = '$co_programa' AND ps.co_secretaria = ej.enjid";

            $res = $this->carregar($sql);
            
            $arrDados = array();
            foreach($res as $r)
            {
                array_push($arrDados, $r['codigo']);
            }
        }

        $arrDados = $arrDados ? $arrDados : array();
        return $arrDados;

    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'co_programa' => array('Digits'),
            'nu_codigo_programa' => array(new Zend_Validate_StringLength(array('max' => 20))),
            'ds_programa' => array(new Zend_Validate_StringLength(array('max' => 255))),
            'ds_sigla' => array(new Zend_Validate_StringLength(array('max' => 10))),
            'ds_objetivo' => array('allowEmpty' => true,'Text'),
            'co_situacao' => array('Digits'),
            'ds_link' => array(new Zend_Validate_StringLength(array('max' => 255))),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui
        $this->ds_sigla = strtoupper($this->ds_sigla);

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

}
