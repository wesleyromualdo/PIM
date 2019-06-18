<?php
/**
 * Classe de mapeamento da entidade spo.fontedetalhadasiafi.
 *
 * @version $Id: Fontedetalhadasiafi.php 141952 2018-07-23 17:56:20Z victormachado $
 */

/**
 * Spo_Model_Fontedetalhadasiafi
 *
 * @package  Spo\Model
 * @author Maykel S. Braz <maykelbraz@mec.gov.br>
 */
class Spo_Model_Fontedetalhadasiafi extends Modelo
{
    /**
     * @var string Nome da tabela especificada.
     */
    protected $stNomeTabela = 'spo.fontedetalhadasiafi';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'fdscod',
        'fdsano',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'fdscod' => null,
        'fdsano' => null,
        'iduso' => null,
        'grfcod' => null,
        'foncod' => null,
        'fdsdsc' => null,
    );

    public function exists($fdscod = null, $fdsano = null){
        $fdscod = empty($fdscod) ? $this->fdscod : $fdscod;
        $fdsano = empty($fdsano) ? $this->fdsano : $fdsano;

        $sql = <<<SQL
          SELECT fdscod FROM spo.fontedetalhadasiafi WHERE fdscod = '%s' AND fdsano = '%s';
SQL;
        $sql = sprintf($sql, $fdscod, $fdsano);

        return empty($this->pegaUm($sql)) ? false : true;
    }

    private function validaChavePrimaria() {
        if (count($this->arChavePrimaria) > 1) {
            foreach ($this->arChavePrimaria as $chave) {
                if (empty($this->$chave)) {
                    return false;
                }
            }
        } else {
            $chave = $this->arChavePrimaria[0];
            return empty($this->$chave);
        }
        return true;
    }

    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array(), $manterAspas = false){
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();

        if( $boAntesSalvar ){
            if( !$this->antesSalvar() ){ return false; }
        }

//        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor declarar atributo 'protected \$arChavePrimaria = [];' na classe filha!" );

        $stChavePrimaria = $this->arChavePrimaria[0];

        if( $this->validaChavePrimaria() && $this->exists() && !$this->tabelaAssociativa ){
            $this->alterar($arCamposNulo);
            $resultado = $this->$stChavePrimaria;
        }else{
            if($manterAspas === false){
                $resultado = $this->inserir($arCamposNulo);
            }else{
                $resultado = $this->inserirManterAspas($arCamposNulo);
            }
        }
        if( $resultado ){
            if( $boDepoisSalvar ){
                $this->depoisSalvar();
            }
        }
        return $resultado;
    } // Fim salvar()

    public function alterar($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
//        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

        $campos = "";
        $whereChave = "";
        foreach( $this->arAtributos as $campo => $valor ){
            if( $valor != null ){
//                if( $campo == $this->arChavePrimaria[0] ){
                if ( in_array($campo, $this->arChavePrimaria) ) {
                    if (!empty($whereChave)) { $whereChave .= " AND "; }
                    $whereChave .= $campo." = '".$valor."'";
                    continue;
                }

                $valor = pg_escape_string( $valor );

                $campos .= $campo." = '".$valor."', ";
            }
            else{
                if(in_array($campo, $arCamposNulo)) {
                    $campos .= $campo." = null, ";
                }
            }
        }

        $campos = substr( $campos, 0, -2 );

        if(!empty($campos) && !empty($this->arChavePrimaria) && $whereChave){
            $sql = " UPDATE $this->stNomeTabela SET $campos WHERE {$whereChave} ";
            //var_dump($sql); exit();
            return $this->executar( $sql );
        } else {
            return false;
        }
    } // Fim _alterar()

    public function inserir($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
//        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );
        $arCampos  = array();
        $arValores = array();
        $arSimbolos = array();

        $troca = array("'", "\\");
        foreach( $this->arAtributos as $campo => $valor ){
            if( $campo == $this->arChavePrimaria[0] && !count($this->arChavePrimaria) > 1 && !$this->tabelaAssociativa ) continue;
            if( $valor !== null ){
                if( !$valor && in_array($campo, $arCamposNulo) ){ continue; }
                $arCampos[]  = $campo;
                $valor = str_replace($troca, "", $valor);
                $arValores[] = trim( pg_escape_string( $valor ) );
            }
        }

        if( count( $arValores ) ){
            $sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
											  values ( '". implode( "', '", $arValores ) ."' )
					 returning {$this->arChavePrimaria[0]}";
            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->$stChavePrimaria = $this->pegaUm( $sql );
        }
    } // Fim _inserir()

    public function inserirManterAspas($arCamposNulo = array())
    {
        $arCamposNulo = is_array($arCamposNulo) ? $arCamposNulo : array();
//        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

        $arCampos  = array();
        $arValores = array();
        $arSimbolos = array();

        foreach( $this->arAtributos as $campo => $valor ){
            if( $campo == $this->arChavePrimaria[0] && !count($this->arChavePrimaria) > 1 && !$this->tabelaAssociativa ) continue;
            if( $valor !== null ){
                if( !$valor && in_array($campo, $arCamposNulo) ){ continue; }
                $arCampos[]  = $campo;
                $arValores[] = trim( pg_escape_string( $valor ) );
            }
        }

        if( count( $arValores ) ){
            $sql = " insert into $this->stNomeTabela ( ". implode( ', ', $arCampos   ) ." )
											  values ( '". implode( "', '", $arValores ) ."' )
					 returning {$this->arChavePrimaria[0]}";
            $stChavePrimaria = $this->arChavePrimaria[0];
            return $this->$stChavePrimaria = $this->pegaUm( $sql );
        }
    } // Fim _inserirManterAspas()

    public function excluir( $id = null, $retorno = null ){
        $complemento = ";";
//        if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

        if($retorno){
            $complemento = "returning $retorno;";
        }

        if( !$this->antesExcluir($id) ) return false;

        $where = '';
        if (count($this->arChavePrimaria) > 1) {
            foreach ($this->arChavePrimaria as $key => $chave) {
                if (!empty($where)) { $where .= ' AND '; }
                if (!empty($id)) {
                    $where .= $chave . " = '" . $id[$key] . "'";
                } else {
                    $where .= $chave . " = '" . $this->arAtributos[$chave] . "'";
                }
            }
        } else {
            $stChavePrimaria = $this->arChavePrimaria[0];
            $id = $id ? $id : $this->$stChavePrimaria;
            $where = $stChavePrimaria." = ".$id;
        }

        $sql = " DELETE FROM $this->stNomeTabela WHERE $where $complemento";

        if($retorno){
            return $this->pegaUm( $sql );
        }else{
            return $this->executar( $sql );
        }
    }

    public function carregarPorId($id, $tempocache = null)
    {
        if (is_array($id)) {
            $where = '';
            foreach ($this->arChavePrimaria as $key => $chave) {
                if (!empty($where)) { $where .= ' AND '; }
                $where .= $chave." = '".$id[$key]."'";
            }

            $sql = " SELECT * FROM $this->stNomeTabela WHERE ".$where;
            $arResultado = $this->pegaLinha($sql, 0, $tempocache);
            $this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
        }

        if (is_numeric($id)) {
            $id = trim( str_replace("'", "", (string) $id));
            $sql = " SELECT * FROM $this->stNomeTabela WHERE {$this->arChavePrimaria[0]} = '$id'; ";

            $arResultado = $this->pegaLinha($sql, 0, $tempocache);
            $this->popularObjeto( array_keys( $this->arAtributos ), $arResultado );
        }
        return $this;
    }

    public static function getQueryCombo($exercicio, $whereAdicional = '')
    {
        $query = <<<DML
SELECT fds.fdscod AS codigo,
       fds.fdscod || ' - ' || fds.fdsdsc AS descricao
  FROM spo.fontedetalhadasiafi fds
  WHERE fds.fdsano = '%d'
    %s
  ORDER BY fds.fdscod
DML;
        return sprintf($query, $exercicio, $whereAdicional);
    }

    public function retornaListaGestaoFonte($fdscod = null){
        $wFdscod = '';

        if (!empty($fdscod)) {
            $wFdscod = " AND fdscod = '".$fdscod."'";
        }

        $sql = <<<SQL
            SELECT
                fdscod as codigo,
                fdsano,
                fdscod,
                fdsdsc
            FROM spo.fontedetalhadasiafi
            WHERE 1=1 %s
            ORDER BY fdsano DESC, fdsdsc;
SQL;
        $sql = sprintf($sql, $wFdscod);
        return $sql;
    }
}
